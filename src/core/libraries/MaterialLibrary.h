#pragma once
#include "core/libraries/ShaderLibrary.h"
#include "core/libraries/TextureLibrary.h"
#include "core/materials/Material.h"
#include <memory>
#include <string>
#include <unordered_map>

class MaterialLibrary
{
public:
  explicit MaterialLibrary() {}

  Material* Add(std::unique_ptr<Material> material);
  Material* Get(const std::string& uuid) const noexcept;
  void Remove(const std::string& uuid);

private:
  std::unordered_map<std::string, std::unique_ptr<Material>> m_Cache;
};
