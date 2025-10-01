#include <terrakit/core/libraries/MaterialLibrary.h>

using namespace terrakit::renderer;

namespace terrakit::core {
std::shared_ptr<Material>
MaterialLibrary::Add(std::shared_ptr<Material> material)
{
  if (!material)
    return nullptr;

  const std::string id = material->uuid;

  if (auto it = m_Cache.find(id); it != m_Cache.end())
    return it->second;

  auto [it, inserted] = m_Cache.emplace(id, material);
  return inserted ? it->second : nullptr;
}

std::shared_ptr<Material>
MaterialLibrary::Get(const std::string& uuid) const
{
  if (auto it = m_Cache.find(uuid); it != m_Cache.end())
    return it->second;
  return nullptr;
}

void
MaterialLibrary::Remove(const std::string& uuid)
{
  m_Cache.erase(uuid);
}
} // namespace terrakit::core