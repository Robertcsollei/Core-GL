#include <terrakit/core/libraries/MeshLibrary.h>

namespace terrakit::core {
std::shared_ptr<Mesh>
MeshLibrary::Add(const std::string& key, std::shared_ptr<Mesh> mesh)
{
  if (!mesh)
    return nullptr;

  if (auto it = m_Cache.find(key); it != m_Cache.end())
    return it->second;

  auto [it, inserted] = m_Cache.emplace(key, mesh);
  return inserted ? it->second : nullptr;
}

std::shared_ptr<Mesh>
MeshLibrary::Get(const std::string& key) const
{
  if (auto it = m_Cache.find(key); it != m_Cache.end()) {
    return it->second;
  }
  return nullptr;
}
} // namespace terrakit::core