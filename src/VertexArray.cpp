#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
	: m_RendererID(0), m_VertexBufferIndex(0), m_IndexBufferIndex(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID)); 
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(
			glVertexAttribPointer(
				i, 
				element.count, 
				element.type,
				element.normalized,
				layout.GetStride(),
				(const void*)offset
			)
		);

		 offset += 
			 element.count 
			 * VertexBufferLayoutElement::GetSizeOfType(element.type);
	}
	// This bind index 0 of VAO to currenlty selected BUFFER!

}
