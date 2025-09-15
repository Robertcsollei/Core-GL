#include <terrakit/core/libraries/TextureLibrary.h>

using namespace terrakit::renderer;

namespace terrakit::core {
std::shared_ptr<Texture>
TextureLibrary::load2D(const std::string& path)
{
  auto it = m_Cache.find(path);
  if (it != m_Cache.end())
    return it->second;
  auto tex = std::make_shared<Texture>(path.c_str());
  m_Cache[path] = tex;
  return tex;
}

} // namespace terrakit::core
