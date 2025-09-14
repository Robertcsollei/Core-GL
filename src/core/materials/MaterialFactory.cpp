#include <core/libraries/ShaderLibrary.h>
#include <core/libraries/TextureLibrary.h>
#include <core/materials/MaterialFactory.h>
#include <core/shaders/ShaderNames.h>
#include <memory>

std::unique_ptr<Material>
MaterialFactory::CreatePong(const std::string& texturePath,
                            ShaderLibrary& shaders,
                            TextureLibrary& textures)
{
  auto shader = shaders.get(ShaderNames::Object3dShader);
  auto tex = textures.load2D(texturePath);

  auto mat =
    std::make_unique<Material>("Pong Material", "Pong Material", shader);
  mat->addTexture(1, tex);
  mat->state.depthTest = true;
  mat->state.depthWrite = true;
  mat->state.cull = false;
  return mat;
}

std::unique_ptr<Material>
MaterialFactory::CreatePoint(ShaderLibrary& shaders, TextureLibrary& textures)
{
  auto shader = shaders.get(ShaderNames::PointShader);

  auto mat =
    std::make_unique<Material>("Point Material", "Point Material", shader);
  mat->state.depthTest = true;
  mat->state.depthWrite = true;
  mat->state.cull = false;
  return mat;
}

std::unique_ptr<Material>
MaterialFactory::CreateColor(ShaderLibrary& shaders, TextureLibrary& textures)
{
  auto shader = shaders.get(ShaderNames::ColorShader);

  auto mat =
    std::make_unique<Material>("Color Material", "Color Material", shader);
  mat->state.depthTest = true;
  mat->state.depthWrite = true;
  mat->state.cull = false;
  return mat;
}

std::unique_ptr<Material>
MaterialFactory::CreateAtmosphere(ShaderLibrary& shaders,
                                  TextureLibrary& textures)
{
  auto shader = shaders.get(ShaderNames::AtmosphereShader);

  auto mat = std::make_unique<Material>(
    "Atmosphere Material", "Atmosphere Material", shader);
  mat->state.depthTest = true;
  mat->state.depthWrite = false;
  mat->state.cull = false;
  mat->state.blending = true;
  mat->state.blendSrc = GL_ONE;
  mat->state.blendDst = GL_ONE;
  return mat;
}