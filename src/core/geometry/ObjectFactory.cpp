#include "core/geometry/ObjectFactory.h"
#include "core/geometry/MeshFactory.h"
#include <core/libraries/ShaderLibrary.h>
#include <core/libraries/TextureLibrary.h>
#include <core/materials/MaterialFactory.h>

std::unique_ptr<Renderable>
ObjectFactory::CreateGlobe(const GlobeConfig& cfg, AppContext& ctx)
{
  auto mesh = ctx.meshes.Add(
    "ellipsoid:globe",
    MeshFactory::CreateEllipsoid(
      cfg.radius, cfg.slices, cfg.stacks, glm::vec3(0.f), *cfg.ellipsoid));

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
    mesh =
      ctx.meshes.Add("point:satellite",
                     MeshFactory::CreatePoint(glm::vec3(0.f), glm::vec4(1.f)));

    mesh->AddInstanceBuffer(nullptr,
                            25000 * sizeof(glm::vec3),
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
