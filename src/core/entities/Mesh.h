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
  struct PointVertex
  {
    glm::vec3 Position;
    glm::vec4 Color;

    PointVertex()
      : Position(0.f)
      , Color(1.f)
    {
    }

    PointVertex(const glm::vec3& position, const glm::vec4& color)
      : Position(position)
      , Color(color)
    {
    }

    static void AppendLayout(VertexBufferLayout* layout)
    {
      layout->Push<float>(3); // Position
      layout->Push<float>(4); // Color
    }
  };

  VertexArray vao;
  VertexBuffer vbo;
  VertexBufferLayout layout;
  VertexBuffer instanceVbos[2];
  VertexBufferLayout instanceLayout;
  IndexBuffer ibo;

  glm::vec3 aabbMin{ FLT_MAX };
  glm::vec3 aabbMax{ -FLT_MAX };
  glm::vec3 center{ 0.0f };

  int currentInstanceVbo = 0;

  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<uint32_t>& indices);

  Mesh(const PointVertex& pointVtx, const std::vector<uint32_t>& indices);

  void AddInstanceBuffer(const PointVertex* pointVtx,
                         size_t size,
                         int attribIndex);
  // TODO: cache vertices and indices in CPU memory for raycasting
};