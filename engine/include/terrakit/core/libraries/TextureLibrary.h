#pragma once
#include <memory>
#include <string>
#include <terrakit/renderer/Texture.h>
#include <unordered_map>
namespace terrakit::core {
class TextureLibrary
{
public:
  std::shared_ptr<terrakit::renderer::Texture> Load(const std::string& path);
  std::shared_ptr<terrakit::renderer::Texture> Get(const std::string& path);
private:
  std::unordered_map<std::string, std::shared_ptr<terrakit::renderer::Texture>>
    m_Cache;
};
} // namespace terrakit::core