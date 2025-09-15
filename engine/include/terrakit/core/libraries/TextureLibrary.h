#pragma once
#include <memory>
#include <string>
#include <terrakit/renderer/Texture.h>
#include <unordered_map>
namespace terrakit::core {
class TextureLibrary
{
public:
  std::shared_ptr<terrakit::renderer::Texture> load2D(
    const std::string& path); // caches by path
private:
  std::unordered_map<std::string, std::shared_ptr<terrakit::renderer::Texture>>
    m_Cache;
};
} // namespace terrakit::core