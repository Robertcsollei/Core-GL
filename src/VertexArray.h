#pragma once
#include "VertexBuffer.h"


class VertexBufferLayout;

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

private:
	unsigned int m_RendererID;
	unsigned int m_VertexBufferIndex;
	unsigned int m_IndexBufferIndex;
};