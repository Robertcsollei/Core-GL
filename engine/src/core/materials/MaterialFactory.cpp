#include "core/materials/MaterialFactory.h"
#include "core/shaders/ShaderNames.h"

#include <memory>
#include <terrakit/core/config/ResourceManager.h>

using namespace terrakit::renderer;
using namespace terrakit::core;
using namespace terrakit::core::config;

std::shared_ptr<Material>
MaterialFactory::CreateTextured3D(const std::string& texturePath, ResourceManager& resources)
{
  auto tex = resources.LoadTexture(texturePath);

  auto mat = std::make_shared<Material>("textured_3d_material", "Textured 3D Object", ShaderNames::Object3dShader);
  if (tex) {
    mat->addTexture(1, tex);
  }
  mat->state.depthTest = true;
  mat->state.depthWrite = true;
  mat->state.cull = false;
  return mat;
}

std::shared_ptr<Material>
MaterialFactory::CreatePoint(ResourceManager& resources)
{
  auto mat = std::make_shared<Material>("point_material", "Point Rendering", ShaderNames::PointShader);
  mat->state.depthTest = true;
  mat->state.depthWrite = true;
  mat->state.cull = false;
  return mat;
}

std::shared_ptr<Material>
MaterialFactory::CreateLine(ResourceManager& resources)
{
  auto mat = std::make_shared<Material>("line_material", "Line Rendering", ShaderNames::LineShader);
  mat->state.depthTest = true;
  mat->state.depthWrite = false;
  mat->state.cull = false;
  return mat;
}

std::shared_ptr<Material>
MaterialFactory::CreateAtmosphere(ResourceManager& resources)
{
  auto mat = std::make_shared<Material>("atmosphere_material", "Atmospheric Effect", ShaderNames::AtmosphereShader);
  mat->state.depthTest = true;
  mat->state.depthWrite = false;
  mat->state.cull = false;
  mat->state.blending = true;
  mat->state.blendSrc = GL_ONE;
  mat->state.blendDst = GL_ONE;
  return mat;
}