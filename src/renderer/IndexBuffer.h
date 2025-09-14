#pragma once

class IndexBuffer
{
public:
  IndexBuffer(const void* data, unsigned int count);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  void SetData(const void* data, unsigned int count);

  unsigned int GetRendererID() const;
  unsigned int GetCount() const;

  // Deleted copy constructor and assignment operator to prevent copying
  IndexBuffer(const IndexBuffer&) = delete;
  IndexBuffer& operator=(const IndexBuffer&) = delete;

private:
  unsigned int m_RendererID;
  unsigned int m_Count;
};