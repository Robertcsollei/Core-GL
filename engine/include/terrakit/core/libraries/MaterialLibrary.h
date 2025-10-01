#pragma once
#include <memory>
#include <string>
#include <terrakit/core/libraries/ShaderLibrary.h>
#include <terrakit/core/libraries/TextureLibrary.h>
#include <terrakit/renderer/Material.h>
#include <unordered_map>

namespace terrakit::core {
class MaterialLibrary
{
public:
  explicit MaterialLibrary() {}

  std::shared_ptr<terrakit::renderer::Material> Add(std::shared_ptr<terrakit::renderer::Material> material);
  std::shared_ptr<terrakit::renderer::Material> Get(const std::string& uuid) const;
  void Remove(const std::string& uuid);

private:
  std::unordered_map<std::string, std::shared_ptr<terrakit::renderer::Material>>
    m_Cache;
};
} // namespace terrakit::core
