#include <terrakit/core/libraries/TextureLibrary.h>
#include <iostream>
#include <stdexcept>

using namespace terrakit::renderer;

namespace terrakit::core {
std::shared_ptr<Texture>
TextureLibrary::load2D(const std::string& path)
{
  auto it = m_Cache.find(path);
  if (it != m_Cache.end())
    return it->second;

  try {
    auto tex = std::make_shared<Texture>(path.c_str());
    m_Cache[path] = tex;
    return tex;
  } catch (const std::runtime_error& e) {
    std::cerr << "Warning: Failed to load texture '" << path << "': " << e.what() << std::endl;
    return nullptr;
  }
}

} // namespace terrakit::core
