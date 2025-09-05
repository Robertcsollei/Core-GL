#include "core/libraries/MaterialLibrary.h"

Material*
MaterialLibrary::Add(std::unique_ptr<Material> material)
{
  if (!material)
    return nullptr;

  const std::string id = material->uuid;

  if (auto it = m_Cache.find(id); it != m_Cache.end())
    return it->second.get();

  auto [it, inserted] = m_Cache.emplace(id, std::move(material));
  return inserted ? it->second.get() : nullptr;
}

Material*
MaterialLibrary::Get(const std::string& uuid) const noexcept
{
  if (auto it = m_Cache.find(uuid); it != m_Cache.end())
    return it->second.get();
  return nullptr;
}

void
MaterialLibrary::Remove(const std::string& uuid)
{
  m_Cache.erase(uuid);
}
