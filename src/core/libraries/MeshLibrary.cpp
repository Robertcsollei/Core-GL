#include "MeshLibrary.h"

Mesh*
MeshLibrary::Add(std::string key, std::unique_ptr<Mesh> mesh)
{
  auto raw = mesh.get();
  m_Cache[std::move(key)] = std::move(mesh);
  return raw;
}

Mesh*
MeshLibrary::Get(std::string_view key) const noexcept
{
  if (auto it = m_Cache.find(std::string(key)); it != m_Cache.end()) {
    return it->second.get();
  }
  return nullptr;
}