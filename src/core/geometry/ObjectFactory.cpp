#include <core/SceneState.h>
#include <core/entities/objects/Satellite.h>
#include <core/geometry/MeshFactory.h>
#include <core/geometry/ObjectFactory.h>
#include <core/libraries/ShaderLibrary.h>
#include <core/libraries/TextureLibrary.h>
#include <core/materials/MaterialFactory.h>

std::unique_ptr<Renderable>
ObjectFactory::CreateGlobe(const GlobeConfig& cfg, AppContext& ctx)
{
  auto mesh =
    ctx.meshes.Add("ellipsoid:globe",
                   MeshFactory::CreateEllipsoid(
                     cfg.slices, cfg.stacks, glm::vec3(0.f), *cfg.ellipsoid));

  auto material = ctx.materials.Add(
    MaterialFactory::CreatePong(cfg.texturePath, ctx.shaders, ctx.textures));

  auto renderable = std::make_unique<Renderable>();
  renderable->mesh = mesh;
  renderable->material = material;
  Transform t{};
  t.translation = cfg.center;
  t.scale = { 1, 1, 1 };
  t.quat = glm::quat(glm::vec3(0, 0, 0));
  renderable->transform = std::move(t);

  return renderable;
}

std::unique_ptr<Renderable>
ObjectFactory::CreateSatellite(const glm::vec3 pos, AppContext& ctx)
{
  auto mesh = ctx.meshes.Get("point:satellite");
  if (!mesh) {
    mesh = ctx.meshes.Add("point:satellite",
                          MeshFactory::CreatePoint(glm::vec3(0.f), 0xFFFFFFFF));

    mesh->AddInstanceBuffer(nullptr,
                            15000 * sizeof(Mesh::PointVertex),
                            2); // location=2 in shader
  }

  auto material =
    ctx.materials.Add(MaterialFactory::CreatePoint(ctx.shaders, ctx.textures));

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

static glm::vec3 color = glm::vec3(0.1f, 0.8f, 0.3f);

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

  auto material = ctx.materials.Get("Color Material");
  if (!material) {
    material = ctx.materials.Add(
      MaterialFactory::CreateColor(ctx.shaders, ctx.textures));

    material->uniforms.push_back(
      { Material::UniformBinding::Type::Vec3, &color, "u_Color" });
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