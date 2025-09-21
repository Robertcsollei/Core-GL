#pragma once
#include <glad/glad.h>

namespace terrakit::renderer {
class VertexBuffer
{
public:
  VertexBuffer();
  VertexBuffer(const void* data, unsigned int size);
  VertexBuffer(const void* data, unsigned int size, unsigned int usage);
  ~VertexBuffer() { Destroy(); }

  VertexBuffer(VertexBuffer&& other) noexcept { MoveFrom(other); }
  VertexBuffer& operator=(VertexBuffer&& other) noexcept
  {
    if (this != &other) {
      Destroy();
      MoveFrom(other);
    }
    return *this;
  }

  // Deleted copy constructor and assignment operator to prevent copying
  VertexBuffer(const VertexBuffer&) = delete;
  VertexBuffer& operator=(const VertexBuffer&) = delete;

public:
  GLuint rendererID() const { return m_RendererID; }
  GLsizeiptr capacity() const { return m_Capacity; }
  GLsizeiptr size() const { return m_Size; }
  GLenum usage() const { return m_Usage; }

  void Bind() const;
  void Unbind() const;
  void SetData(const void* data, unsigned int size);
  void UpdateSubData(GLintptr offset, GLsizeiptr size, const void* data);
  void Orphan();

private:
  GLuint m_RendererID = 0;
  GLsizeiptr m_Capacity = 0; // allocated bytes
  GLsizeiptr m_Size = 0;     // valid data bytes
  GLenum m_Usage = GL_STATIC_DRAW;

private:
  void Destroy();
  void MoveFrom(VertexBuffer& o);
  void Allocate(GLsizeiptr size);
  void EnsureCapacity(GLsizeiptr size);
};
} // namespace terrakit::renderer