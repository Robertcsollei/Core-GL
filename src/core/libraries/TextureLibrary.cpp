#include "core/libraries/TextureLibrary.h"

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
