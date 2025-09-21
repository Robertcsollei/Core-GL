#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <terrakit/renderer/Renderer.h>
#include <terrakit/renderer/Texture.h>

using namespace terrakit::renderer;

Texture::Texture(const std::string& path)
  : m_RendererID(0)
  , m_FilePath(path)
  , m_LocalBuffer(nullptr)
  , m_Width(0)
  , m_Height(0)
  , m_BPP(0)
{
  // stbi_set_flip_vertically_on_load(true);
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

  if (!m_LocalBuffer) {
    std::cerr << "Failed to load texture: " << stbi_failure_reason() << "\n"
              << "Path: " << path << "\n"
              << "Working directory: " << std::filesystem::current_path() << std::endl;
    throw std::runtime_error("Failed to load texture: " + path);
  }

  // Texture tiling can be set using glTexParameteri
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  // GL_REPEAT | GL_MIRRORED_REPEAT | GL_CLAMP_TO_EDGE | GL_CLAMP_TO_BORDER

  GLCall(glGenTextures(1, &m_RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexImage2D(GL_TEXTURE_2D,
                      0,
                      GL_RGBA8,
                      m_Width,
                      m_Height,
                      0,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      m_LocalBuffer));
  glGenerateMipmap(GL_TEXTURE_2D);
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (m_LocalBuffer) {
    stbi_image_free(m_LocalBuffer);
  }
}

Texture::~Texture()
{
  GLCall(glDeleteTextures(1, &m_RendererID));
}

void
Texture::Bind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void
Texture::Unbind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture*
Texture::Create(const std::string& path)
{
  return nullptr;
}
