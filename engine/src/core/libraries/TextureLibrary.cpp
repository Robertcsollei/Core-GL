#include <terrakit/core/libraries/TextureLibrary.h>
#include <terrakit/core/Logger.h>
#include <iostream>
#include <stdexcept>

using namespace terrakit::renderer;

namespace terrakit::core {
std::shared_ptr<Texture>
TextureLibrary::Load(const std::string& path)
{
  auto it = m_Cache.find(path);
  if (it != m_Cache.end())
    return it->second;

  try {
    auto tex = std::make_shared<Texture>(path.c_str());
    m_Cache[path] = tex;
    return tex;
  } catch (const std::runtime_error& e) {
    TK_ERROR(std::string("Warning: Failed to load texture '") + path + std::string("': " ) + std::string(e.what()));
    return nullptr;
  }
}

std::shared_ptr<Texture>
TextureLibrary::Get(const std::string& path)
{
  auto it = m_Cache.find(path);
  if (it != m_Cache.end())
    return it->second;

  return nullptr; // Don't auto-load, just return cached
}

} // namespace terrakit::core
