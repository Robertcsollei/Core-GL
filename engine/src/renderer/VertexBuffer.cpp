#include <terrakit/renderer/Renderer.h>
#include <terrakit/renderer/VertexBuffer.h>

using namespace terrakit::renderer;

VertexBuffer::VertexBuffer()
  : VertexBuffer(nullptr, 0, GL_STATIC_DRAW)
{
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
  : VertexBuffer(data, size, GL_STATIC_DRAW)
{
}

VertexBuffer::VertexBuffer(const void* data,
                           unsigned int size,
                           unsigned int usage)
  : m_RendererID(0)
  , m_Capacity(0)
  , m_Size(0)
  , m_Usage(usage)
{
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  Allocate(size);
  if (data)
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void
VertexBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void
VertexBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void
VertexBuffer::SetData(const void* data, unsigned int size)
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void
VertexBuffer::UpdateSubData(GLintptr offset, GLsizeiptr size, const void* data)
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void
VertexBuffer::Orphan()
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ARRAY_BUFFER, m_Capacity, nullptr, m_Usage));
}

void
VertexBuffer::destroy()
{
  if (m_RendererID) {
    GLCall(glDeleteBuffers(1, &m_RendererID));
    m_RendererID = 0;
  }
  m_Capacity = m_Size = 0;
}

void
VertexBuffer::moveFrom(VertexBuffer& o)
{
  m_RendererID = o.m_RendererID;
  o.m_RendererID = 0;
  m_Capacity = o.m_Capacity;
  o.m_Capacity = 0;
  m_Size = o.m_Size;
  o.m_Size = 0;
  m_Usage = o.m_Usage;
}

void
VertexBuffer::Allocate(GLsizeiptr size)
{
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, m_Usage));
  m_Capacity = size;
  m_Size = size;
}

void
VertexBuffer::ensureCapacity(GLsizeiptr size)
{
  if (size <= m_Capacity)
    return;
  // grow (e.g., 1.5x) to avoid frequent reallocations
  GLsizeiptr newCap = (m_Capacity > 0) ? (m_Capacity + m_Capacity / 2) : size;
  if (newCap < size)
    newCap = size;
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ARRAY_BUFFER, newCap, nullptr, m_Usage));
  m_Capacity = newCap;
}
