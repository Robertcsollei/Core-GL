#include <core/entities/Mesh.h>

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<uint32_t>& indices)
  : vao()
  , vbo(vertices.data(), vertices.size() * sizeof(Vertex))
  , ibo(indices.data(), indices.size())
  , layout()
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

Mesh::Mesh(const std::vector<PointVertex>& vertices,
           const std::vector<uint32_t>& indices)
  : vao()
  , vbo(vertices.data(), vertices.size() * sizeof(PointVertex))
  , ibo(indices.data(), indices.size())
  , layout()
{
  Mesh::PointVertex::AppendLayout(&layout);
  vao.addVertexBuffer(vbo, layout);
  vao.SetIndexBuffer(ibo);

  for (const auto& v : vertices) {
    aabbMin = glm::min(aabbMin, v.Position);
    aabbMax = glm::max(aabbMax, v.Position);
  }
  center = 0.5f * (aabbMin + aabbMax);
}

Mesh::Mesh(const PointVertex& pointVtx, const std::vector<uint32_t>& indices)
  : vao()
  , vbo(&pointVtx, sizeof(PointVertex))
  , ibo(indices.data(), indices.size())
  , layout()
{
  Mesh::PointVertex::AppendLayout(&layout);
  vao.addVertexBuffer(vbo, layout);
  vao.SetIndexBuffer(ibo);

  aabbMin = aabbMax = pointVtx.Position;
  center = pointVtx.Position;
}

void
Mesh::AddInstanceBuffer(const PointVertex* pointVtx,
                        size_t size,
                        int attribIndex)
{
  instanceVbo = VertexBuffer(pointVtx, size, GL_STREAM_DRAW);

  VertexBufferLayout instLayout;
  PointVertex::AppendLayout(&instLayout);

  vao.addVertexBuffer(instanceVbo, instLayout);

  glVertexAttribDivisor(attribIndex, 1);
  glVertexAttribDivisor(attribIndex + 1, 1);
}