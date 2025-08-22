#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
  : m_RendererID(0)
  , m_Count(count)
{
  ASSERT(sizeof(unsigned int) == sizeof(GLuint))

  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      count * sizeof(unsigned int),
                      data,
                      GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
  GLCall(glDeleteBuffers(1, &m_RendererID));
}

void
IndexBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void
IndexBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void
IndexBuffer::SetData(const void* data, unsigned int size)
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data));
}

unsigned int
IndexBuffer::GetRendererID() const
{
  return m_RendererID;
}

unsigned int
IndexBuffer::GetCount() const
{
  return m_Count;
}