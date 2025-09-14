#pragma once
#include <core/materials/Material.h>

class ShaderLibrary;
class TextureLibrary;

namespace MaterialFactory {
std::unique_ptr<Material>
CreatePong(const std::string& texturePath,
           ShaderLibrary& shaders,
           TextureLibrary& textures);

std::unique_ptr<Material>
CreatePoint(ShaderLibrary& shaders, TextureLibrary& textures);

std::unique_ptr<Material>
CreateColor(ShaderLibrary& shaders, TextureLibrary& textures);

std::unique_ptr<Material>
CreateAtmosphere(ShaderLibrary& shaders, TextureLibrary& textures);
}