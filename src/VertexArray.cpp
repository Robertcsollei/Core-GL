#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
  GLCall(glGenVertexArrays(1, &m_RendererID));
  GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
  GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void
VertexArray::Bind() const
{
  GLCall(glBindVertexArray(m_RendererID));
}

void
VertexArray::Unbind() const
{
  GLCall(glBindVertexArray(0));
}

void
VertexArray::SetIndexBuffer(const IndexBuffer& ib)
{
  Bind();
  ib.Bind();
  m_IndexBuffer = &ib;
}

void
VertexArray::addVertexBuffer(const VertexBuffer& vb,
                             const VertexBufferLayout& layout)
{
  Bind();
  vb.Bind();

  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const GLuint attrib = m_NextAttribIndex++;
    GLCall(glEnableVertexAttribArray(attrib));
    const auto& element = elements[i];

    const void* ptr = reinterpret_cast<const void*>(offset);
    GLCall(glVertexAttribPointer(attrib,
                                 element.count,
                                 element.type,
                                 element.normalized,
                                 layout.GetStride(),
                                 (const void*)offset));

    offset +=
      element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
  }

  m_NextAttribIndex = 0;
  // This bind index 0 of VAO to currenlty selected BUFFER!
}
