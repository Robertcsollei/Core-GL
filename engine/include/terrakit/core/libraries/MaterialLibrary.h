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

  terrakit::renderer::Material* Add(
    std::unique_ptr<terrakit::renderer::Material> material);
  terrakit::renderer::Material* Get(const std::string& uuid) const noexcept;
  void Remove(const std::string& uuid);

private:
  std::unordered_map<std::string, std::unique_ptr<terrakit::renderer::Material>>
    m_Cache;
};
} // namespace terrakit::core
