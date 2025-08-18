#pragma once
#include <vector>
#include "Renderer.h"
#include <stdexcept>

struct VertexBufferLayoutElement
{
	unsigned int type;
	unsigned int count; 
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT:         return sizeof(float);
			case GL_UNSIGNED_INT:  return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		}
		ASSERT(false); // Unsupported type
		return 0;
	}
};

class VertexBufferLayout 
{
public:
	VertexBufferLayout() : m_Stride(0) {};
	~VertexBufferLayout() = default;

	template<typename T>
	void Push(unsigned int count) {
		std::runtime_error();
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int GetStride() const { return m_Stride; }
	inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_Elements; }

private:
	std::vector<VertexBufferLayoutElement> m_Elements;
	unsigned int m_Stride;



};