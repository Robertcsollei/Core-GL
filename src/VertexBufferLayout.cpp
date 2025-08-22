#include "VertexBufferLayout.h"
#include "Renderer.h"

unsigned int
VertexBufferLayoutElement::GetSizeOfType(unsigned int type)
{
  switch (type) {
    case GL_FLOAT:
      return sizeof(float);
    case GL_UNSIGNED_INT:
      return sizeof(unsigned int);
    case GL_UNSIGNED_BYTE:
      return sizeof(unsigned char);
  }
  ASSERT(false); // Unsupported type
  return 0;
}

template<>
void
VertexBufferLayout::Push<float>(unsigned int count)
{
  m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
  m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
}
template<>
void
VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
  m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
  m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void
VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
  m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
  m_Stride +=
    count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}