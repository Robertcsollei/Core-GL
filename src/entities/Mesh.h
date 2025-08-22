#pragma once
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include <glm/glm.hpp>
#include <vector>

struct Mesh
{
  struct Vertex
  {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 Color;

    Vertex(const glm::vec3& position,
           const glm::vec3& normal,
           const glm::vec2& texCoords,
           const glm::vec4& color)
      : Position(position)
      , Normal(normal)
      , TexCoords(texCoords)
      , Color(color)
    {
    }

    static void AppendLayout(VertexBufferLayout* layout)
    {
      layout->Push<float>(3); // Position
      layout->Push<float>(3); // Normal
      layout->Push<float>(2); // TexCoord
      layout->Push<float>(4); // Color
    }
  };

  VertexArray vao;
  VertexBuffer vbo;
  VertexBufferLayout layout;
  IndexBuffer ibo;

  glm::vec3 aabbMin{ FLT_MAX };
  glm::vec3 aabbMax{ -FLT_MAX };
  glm::vec3 center{ 0.0f };

  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<uint32_t>& indices)
    : vao()
    , vbo(vertices.data(), vertices.size() * sizeof(Vertex))
    , ibo(indices.data(), indices.size())
  {
    Mesh::Vertex::AppendLayout(&layout);
    vao.addVertexBuffer(vbo, layout);
    vao.SetIndexBuffer(ibo);

    for (const auto& v : vertices) {
      aabbMin = glm::min(aabbMin, v.Position);
      aabbMax = glm::max(aabbMax, v.Position);
    }
    center = 0.5f * (aabbMin + aabbMax);
  }
};