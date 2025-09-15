#pragma once
#include <memory>
#include <string>
#include <terrakit/core/Mesh.h>
#include <unordered_map>

namespace terrakit::core {
class MeshLibrary
{
public:
  MeshLibrary() = default;
  ~MeshLibrary() = default;

public:
  Mesh* Add(std::string key, std::unique_ptr<Mesh> mesh);
  Mesh* Get(std::string_view key) const noexcept;

  void clear() { m_Cache.clear(); }

private:
  std::unordered_map<std::string, std::unique_ptr<Mesh>> m_Cache;
};
} // namespace terrakit::core