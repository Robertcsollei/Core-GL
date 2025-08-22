#pragma once
#include "../VertexBufferLayout.h"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <string>

namespace test {

struct Vertex
{
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoord;

  Vertex(const glm::vec3& position, const glm::vec4& color)
    : Position(position)
    , Color(color)
    , TexCoord(0.f, 0.f)
  {
  }

  Vertex(const glm::vec3& position,
         const glm::vec4& color,
         const glm::vec2& textCoord)
    : Position(position)
    , Color(color)
    , TexCoord(textCoord)
  {
  }

  static void AppendLayout(VertexBufferLayout* layout)
  {
    layout->Push<float>(3); // Position
    layout->Push<float>(4); // Color
    layout->Push<float>(2); // TexCoord
  }
};

static inline std::array<Vertex, 4>
CreateRect(float x,
           float y,
           float w,
           float h,
           const glm::vec4& color,
           float depth = 0.f)
{
  return { Vertex(glm::vec3(x, y, depth), color, { 0.f, 0.f }),
           Vertex(glm::vec3(x + w, y, depth), color, { 1.f, 0.f }),
           Vertex(glm::vec3(x + w, y - h, depth), color, { 1.f, 1.f }),
           Vertex(glm::vec3(x, y - h, depth), color, { 0.f, 1.f }) };
};

static inline std::vector<uint32_t>
CreateRectIndices(size_t vertexCount)
{
  std::vector<uint32_t> indices;
  const size_t quadCount = vertexCount / 4;
  indices.reserve(quadCount * 6);
  for (int q = 0; q < quadCount; q++) {
    const uint32_t base = static_cast<uint32_t>(q * 4);
    indices.push_back(base + 0);
    indices.push_back(base + 1);
    indices.push_back(base + 2);
    indices.push_back(base + 2);
    indices.push_back(base + 3);
    indices.push_back(base + 0);
  }

  return indices;
};

static inline void
AppendBox(std::vector<Vertex>& outVertices,
          std::vector<uint32_t>& outIndices,
          const glm::vec3& center,
          const glm::vec3& size,
          const glm::vec4& color)
{
  const glm::vec3 h = size * 0.5f;
  const glm::vec3 min = center - h;
  const glm::vec3 max = center + h;

  glm::vec3 p[8] = { { min.x, min.y, min.z }, { max.x, min.y, min.z },
                     { min.x, max.y, min.z }, { max.x, max.y, min.z },
                     { min.x, min.y, max.z }, { max.x, min.y, max.z },
                     { min.x, max.y, max.z }, { max.x, max.y, max.z } };

  auto addFace = [&](const glm::vec3& a,
                     const glm::vec3& b,
                     const glm::vec3& c,
                     const glm::vec3& d) {
    const uint32_t base = static_cast<uint32_t>(outVertices.size());

    outVertices.push_back(Vertex{ a, color, { 0.0f, 0.0f } });
    outVertices.push_back(Vertex{ b, color, { 1.0f, 0.0f } });
    outVertices.push_back(Vertex{ c, color, { 1.0f, 1.0f } });
    outVertices.push_back(Vertex{ d, color, { 0.0f, 1.0f } });

    outIndices.push_back(base + 0);
    outIndices.push_back(base + 1);
    outIndices.push_back(base + 2);
    outIndices.push_back(base + 2);
    outIndices.push_back(base + 3);
    outIndices.push_back(base + 0);
  };

  addFace(p[5], p[1], p[3], p[7]);
  addFace(p[0], p[4], p[6], p[2]);
  addFace(p[2], p[6], p[7], p[3]);
  addFace(p[0], p[1], p[5], p[4]);
  addFace(p[4], p[5], p[7], p[6]);
  addFace(p[1], p[0], p[2], p[3]);
}

static inline void
AppendSphere(std::vector<Vertex>& outVertices,
             std::vector<uint32_t>& outIndices,
             const glm::vec3& center,
             float radius,
             int stacks,
             int slices,
             const glm::vec4& color)
{
  const int rows = stacks + 1; // include both poles
  const int cols = slices + 1; // duplicate seam (u=0 and u=1)

  outVertices.reserve(outVertices.size() + rows * cols);
  outIndices.reserve(outIndices.size() + stacks * slices * 6);

  for (int i = 0; i < rows; ++i) {
    // v in [0,1], v=0 at north pole
    float v = static_cast<float>(i) / stacks;
    float lat = (0.5f - v) * glm::pi<float>(); // [-pi/2, +pi/2]

    float sinLat = std::sin(lat);
    float cosLat = std::cos(lat);

    for (int j = 0; j < cols; ++j) {
      // u in [0,1], u=0 at -pi, u=1 at +pi (duplicate seam)
      float u = static_cast<float>(j) / slices;
      float lon = (u - 0.5f) * 2.0f * glm::pi<float>(); // [-pi, +pi]

      float sinLon = std::sin(lon);
      float cosLon = std::cos(lon);

      // Unit sphere direction (x right, y up, z forward)
      glm::vec3 dir = glm::vec3(cosLat * cosLon, sinLat, cosLat * sinLon);

      glm::vec3 pos = center + radius * dir;

      // Equirectangular UVs (v=0 at north, v=1 at south)
      glm::vec2 uv(1.0f - u, v);

      outVertices.push_back(Vertex{ pos, color, uv });
    }
  }

  // Indices (two tris per quad), CCW
  auto idx = [cols](int r, int c) -> uint32_t {
    return static_cast<uint32_t>(r * cols + c);
  };

  for (int i = 0; i < stacks; ++i) {
    for (int j = 0; j < slices; ++j) {
      uint32_t i0 = idx(i, j);
      uint32_t i1 = idx(i, j + 1);
      uint32_t i2 = idx(i + 1, j + 1);
      uint32_t i3 = idx(i + 1, j);

      // Tri 1
      outIndices.push_back(i0);
      outIndices.push_back(i1);
      outIndices.push_back(i2);
      // Tri 2
      outIndices.push_back(i2);
      outIndices.push_back(i3);
      outIndices.push_back(i0);
    }
  }
}
}