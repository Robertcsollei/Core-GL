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
  std::shared_ptr<Mesh> Add(const std::string& key, std::shared_ptr<Mesh> mesh);
  std::shared_ptr<Mesh> Get(const std::string& key) const;

  void Clear() { m_Cache.clear(); }

private:
  std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Cache;
};
} // namespace terrakit::core