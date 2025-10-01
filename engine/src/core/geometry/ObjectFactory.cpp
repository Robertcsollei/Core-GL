#include "core/geometry/ObjectFactory.h"
#include "core/geometry/MeshFactory.h"
#include "core/materials/MaterialFactory.h"
#include <terrakit/core/SceneState.h>
#include <terrakit/core/entities/Satellite.h>
#include <terrakit/core/libraries/ShaderLibrary.h>
#include <terrakit/core/libraries/TextureLibrary.h>

using namespace terrakit::renderer;

namespace terrakit::core {
std::unique_ptr<Renderable>
ObjectFactory::CreateGlobe(const GlobeConfig& cfg, config::ResourceManager& resources)
{
  auto mesh = MeshFactory::CreateEllipsoid(
    cfg.slices, cfg.stacks, glm::vec3(0.f), *cfg.ellipsoid);
  auto meshHandle = resources.CreateMesh("ellipsoid:globe", mesh);

  auto material = MaterialFactory::CreateTextured3D(cfg.textureName, resources);
  auto materialHandle = resources.CreateMaterial(material);

  auto renderable = std::make_unique<Renderable>();
  renderable->mesh = meshHandle;
  renderable->material = materialHandle;
  Transform t{};
  t.translation = cfg.center;
  t.scale = { 1, 1, 1 };
  t.quat = glm::quat(glm::vec3(0, 0, 0));
  renderable->transform = std::move(t);

  return renderable;
}

std::unique_ptr<Renderable>
ObjectFactory::CreateSatellite(const glm::vec3 pos, config::ResourceManager& resources)
{
  auto meshHandle = resources.GetMesh("point:satellite");
  if (!meshHandle.IsValid()) {
    auto mesh = MeshFactory::CreatePoint(glm::vec3(0.f), 0xFFFFFFFF);
    mesh.get()->AddInstanceBuffer(nullptr,
                            15000 * sizeof(Mesh::PointVertex),
                            2); // location=2 in shader
    meshHandle = resources.CreateMesh("point:satellite", mesh);
  }

  auto materialHandle = resources.GetMaterial("point_material");
  if (!materialHandle.IsValid()) {
    auto material = MaterialFactory::CreatePoint(resources);
    materialHandle = resources.CreateMaterial(material);

    // TODO: Uniform binding system needs redesign to decouple from AppContext
    // material->uniforms.push_back({ ... });
  }

  auto renderable = std::make_unique<Renderable>();
  renderable->mesh = meshHandle;
  renderable->material = materialHandle;
  Transform t{};
  t.translation = pos;
  t.scale = { 1, 1, 1 };
  t.quat = glm::quat(glm::vec3(0, 0, 0));
  renderable->transform = std::move(t);

  return renderable;
}

std::unique_ptr<Renderable>
ObjectFactory::CreateOrbit(const std::string name,
                           double dt,
                           const glm::vec3 pos,
                           SceneState& state,
                           AppContext& ctx)
{
  auto mesh = ctx.meshes.Get(name);
  if (!mesh) {
    mesh =
      ctx.meshes.Add(name,
                     MeshFactory::CreateOrbitRibbon(*state.selectedSat,
                                                    dt,
                                                    128,
                                                    0xFF00FF00,
                                                    state.camera.position(),
                                                    0.02f));
  }

  auto material = ctx.materials.Get("Line Material");
  if (!material) {
    material =
      ctx.materials.Add(MaterialFactory::CreateLine(ctx.shaders, ctx.textures));

    material->uniforms.push_back({ Material::UniformBinding::Type::Int,
                                   &ctx.satOptions.selectColor,
                                   "u_Color" });
    material->uniforms.push_back({ Material::UniformBinding::Type::Float,
                                   &ctx.satOptions.orbitThickness,
                                   "u_LineWidth" });
    material->uniforms.push_back({ Material::UniformBinding::Type::Vec2,
                                   &ctx.screenSize,
                                   "u_ScreenSize" });
  }

  auto renderable = std::make_unique<Renderable>();
  renderable->mesh = mesh;
  renderable->material = material;
  Transform t{};
  t.translation = pos;
  t.scale = { 1, 1, 1 };
  t.quat = glm::quat(glm::vec3(0, 0, 0));
  renderable->transform = std::move(t);

  return renderable;
}
} // namespace terrakit::core