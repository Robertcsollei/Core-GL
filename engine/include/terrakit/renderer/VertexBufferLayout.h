#pragma once
#include <stdexcept>
#include <vector>

namespace terrakit::renderer {

struct VertexBufferLayoutElement
{
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
public:
  VertexBufferLayout()
    : m_Stride(0){};
  ~VertexBufferLayout() = default;

  template<typename T>
  void Push(unsigned int count)
  {
    std::runtime_error();
  }

  template<>
  void Push<float>(unsigned int count);

  template<>
  void Push<unsigned int>(unsigned int count);

  template<>
  void Push<unsigned char>(unsigned int count);

  inline unsigned int stride() const { return m_Stride; }
  inline const std::vector<VertexBufferLayoutElement>& elements() const
  {
    return m_Elements;
  }

private:
  std::vector<VertexBufferLayoutElement> m_Elements;
  unsigned int m_Stride;
};
}