#pragma once
#include <glm/glm.hpp>
#include <terrakit/renderer/IndexBuffer.h>
#include <terrakit/renderer/VertexArray.h>
#include <terrakit/renderer/VertexBuffer.h>
#include <terrakit/renderer/VertexBufferLayout.h>
#include <vector>

namespace terrakit::core {
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

    static void AppendLayout(terrakit::renderer::VertexBufferLayout* layout)
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
    uint32_t Color;

    PointVertex()
      : Position(0.f)
      , Color(1.f)
    {
    }

    PointVertex(const glm::vec3& position, uint32_t color)
      : Position(position)
      , Color(color)
    {
    }

    static void AppendLayout(terrakit::renderer::VertexBufferLayout* layout)
    {
      layout->Push<float>(3);         // Position
      layout->Push<unsigned char>(4); // Color
    }
  };
  struct LineVertex
  {
    glm::vec3 Position;
    glm::vec3 NextPosition;
    float Side;

    LineVertex()
      : Position(0.f)
      , NextPosition(0.f)
      , Side(0.f)
    {
    }

    LineVertex(const glm::vec3& pos, const glm::vec3& nextPos, float slide)
      : Position(pos)
      , NextPosition(nextPos)
      , Side(slide)
    {
    }

    static void AppendLayout(terrakit::renderer::VertexBufferLayout* layout)
    {
      layout->Push<float>(3); // Position
      layout->Push<float>(3); // NextPosition
      layout->Push<float>(1); // Side
    }
  };

  terrakit::renderer::VertexArray vao;
  terrakit::renderer::VertexBuffer vbo;
  terrakit::renderer::VertexBufferLayout layout;
  terrakit::renderer::VertexBuffer instanceVbo;
  terrakit::renderer::VertexBufferLayout instanceLayout;
  terrakit::renderer::IndexBuffer ibo;

  glm::vec3 aabbMin{ FLT_MAX };
  glm::vec3 aabbMax{ -FLT_MAX };
  glm::vec3 center{ 0.0f };

  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<uint32_t>& indices);

  Mesh(const std::vector<PointVertex>& vertices,
       const std::vector<uint32_t>& indices);

  Mesh(const PointVertex& pointVtx, const std::vector<uint32_t>& indices);

  Mesh(const std::vector<LineVertex>& vertices,
       const std::vector<uint32_t>& indices);

  void AddInstanceBuffer(const PointVertex* pointVtx,
                         size_t size,
                         int attribIndex);
  // TODO: cache vertices and indices in CPU memory for raycasting
};
} // namespace terrakit::core