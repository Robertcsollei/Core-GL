#include "core/geometry/MeshFactory.h"
#include <core/geometry/Ellipsoid.h>
#include <glm/gtc/constants.hpp>

static inline void
BuildEllipsoid(std::vector<Mesh::Vertex>& outVerts,
               std::vector<uint32_t>& outIdx,
               const glm::vec3& center,
               float radius,
               int stacks,
               int slices,
               Ellipsoid& ellipsoid)
{
  const int rows = stacks + 1;
  const int cols = slices + 1;
  glm::dvec3 radii = ellipsoid.Radii();
  outVerts.reserve(outVerts.size() + rows * cols);
  outIdx.reserve(outIdx.size() + stacks * slices * 6);

  for (int i = 0; i < rows; ++i) {
    double v = static_cast<double>(i) / stacks;
    double lat = (0.5 - v) * glm::pi<double>();
    double sLat = std::sin(lat), cLat = std::cos(lat);

    for (int j = 0; j < cols; ++j) {
      double u = static_cast<double>(j) / slices;
      double lon = (u - 0.5) * 2.0 * glm::pi<double>();
      double sLon = std::sin(lon), cLon = std::cos(lon);

      glm::dvec3 p_local(
        radii.x * cLat * cLon, radii.y * sLat, radii.z * cLat * sLon);

      glm::vec3 pos = glm::vec3(p_local * static_cast<double>(radius)) + center;

      glm::dvec3 n_geo = ellipsoid.GeodeticSurfaceNormal(p_local);
      glm::vec3 n = glm::normalize(glm::vec3(n_geo));

      glm::vec2 uv(1.0f - static_cast<float>(u), static_cast<float>(v));

      outVerts.emplace_back(pos, n, uv, glm::vec4(1.0f));
    }
  }

  auto idx = [cols](int r, int c) -> uint32_t { return r * cols + c; };

  for (int i = 0; i < stacks; ++i) {
    for (int j = 0; j < slices; ++j) {
      uint32_t i0 = idx(i, j);
      uint32_t i1 = idx(i, j + 1);
      uint32_t i2 = idx(i + 1, j + 1);
      uint32_t i3 = idx(i + 1, j);

      outIdx.push_back(i0);
      outIdx.push_back(i1);
      outIdx.push_back(i2);
      outIdx.push_back(i2);
      outIdx.push_back(i3);
      outIdx.push_back(i0);
    }
  }
}

static inline Mesh::PointVertex
BuildPoint(std::vector<uint32_t>& outIdx,
           const glm::vec3& center,
           const glm::vec4& color)
{
  outIdx.push_back(0);
  return Mesh::PointVertex(center, color);
}

std::unique_ptr<Mesh>
MeshFactory::CreateEllipsoid(float radius,
                             int slices,
                             int stacks,
                             const glm::vec3& center,
                             Ellipsoid& ellipsoid)
{
  std::vector<Mesh::Vertex> verts;
  std::vector<uint32_t> indices;

  BuildEllipsoid(verts, indices, center, radius, stacks, slices, ellipsoid);
  return std::make_unique<Mesh>(verts, indices);
}

std::unique_ptr<Mesh>
MeshFactory::CreatePoint(const glm::vec3& center, const glm::vec4& color)
{
  std::vector<uint32_t> indices;

  Mesh::PointVertex vtx = BuildPoint(indices, center, color);
  return std::make_unique<Mesh>(vtx, indices);
}
