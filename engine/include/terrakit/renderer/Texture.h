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

  inline int width() const { return m_Width; }
  inline int height() const { return m_Height; }

  inline unsigned int rendererID() const { return m_RendererID; }

private:
  unsigned int m_RendererID;
  std::string m_FilePath;
  unsigned char* m_LocalBuffer;
  int m_Width, m_Height, m_Bpp;
};
} // namespace terrakit::renderer