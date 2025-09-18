#pragma once
#include <terrakit/renderer/Material.h>

namespace terrakit::core {
class ShaderLibrary;
class TextureLibrary;

namespace MaterialFactory {
std::unique_ptr<terrakit::renderer::Material>
CreatePong(const std::string& texturePath,
           ShaderLibrary& shaders,
           TextureLibrary& textures);

std::unique_ptr<terrakit::renderer::Material>
CreatePoint(ShaderLibrary& shaders, TextureLibrary& textures);

std::unique_ptr<terrakit::renderer::Material>
CreateLine(ShaderLibrary& shaders, TextureLibrary& textures);

std::unique_ptr<terrakit::renderer::Material>
CreateAtmosphere(ShaderLibrary& shaders, TextureLibrary& textures);
} // namespace MaterialFactory
} // namespace terrakit::core