#pragma once
#include <terrakit/renderer/Material.h>

namespace terrakit::core {
namespace config {
class ResourceManager;
}

namespace MaterialFactory {
std::shared_ptr<terrakit::renderer::Material>
CreateTextured3D(const std::string& texturePath,
                 config::ResourceManager& resources);

std::shared_ptr<terrakit::renderer::Material>
CreatePoint(config::ResourceManager& resources);

std::shared_ptr<terrakit::renderer::Material>
CreateLine(config::ResourceManager& resources);

std::shared_ptr<terrakit::renderer::Material>
CreateAtmosphere(config::ResourceManager& resources);
} // namespace MaterialFactory
} // namespace terrakit::core