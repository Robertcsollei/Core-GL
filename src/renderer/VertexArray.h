#pragma once
#include <renderer/IndexBuffer.h>
#include <renderer/VertexBuffer.h>

class VertexBufferLayout;

class VertexArray
{
public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;

  void addVertexBuffer(const VertexBuffer& vb,
                       const VertexBufferLayout& layout);
  void SetIndexBuffer(const IndexBuffer& ib);

  const IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }
  unsigned int GetRendererID() const { return m_RendererID; }

private:
  unsigned int m_RendererID = 0;
  unsigned int m_NextAttribIndex = 0;

  const IndexBuffer* m_IndexBuffer = nullptr;
};