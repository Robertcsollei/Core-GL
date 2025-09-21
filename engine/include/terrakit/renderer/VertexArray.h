#pragma once
#include <terrakit/renderer/IndexBuffer.h>
#include <terrakit/renderer/VertexBuffer.h>

namespace terrakit::renderer {
class VertexBufferLayout;

class VertexArray
{
public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;

  void AddVertexBuffer(const VertexBuffer& vb,
                       const VertexBufferLayout& layout);
  void SetIndexBuffer(const IndexBuffer& ib);

  const IndexBuffer* indexBuffer() const { return m_IndexBuffer; }
  unsigned int rendererID() const { return m_RendererID; }

private:
  unsigned int m_RendererID = 0;
  unsigned int m_NextAttribIndex = 0;

  const IndexBuffer* m_IndexBuffer = nullptr;
};
} // namespace terrakit::renderer