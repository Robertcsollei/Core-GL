#pragma once



class VertexBuffer {
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, unsigned int size);

	unsigned int GetRendererID() const;

	// Deleted copy constructor and assignment operator to prevent copying
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;


private: 
	unsigned int m_RendererID;
};