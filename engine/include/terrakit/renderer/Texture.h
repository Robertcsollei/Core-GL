#pragma once
#include <string>

namespace terrakit::renderer {
class Texture
{
public:
  Texture(const std::string& path);
  ~Texture();

  void Bind() const;
  void Unbind() const;

  inline int GetWidth() const { return m_Width; }
  inline int GetHeight() const { return m_Height; }

  inline unsigned int GetRendererID() const { return m_RendererID; }

private:
  unsigned int m_RendererID;
  std::string m_FilePath;
  unsigned char* m_LocalBuffer;
  int m_Width, m_Height, m_BPP;
};
} // namespace terrakit::renderer