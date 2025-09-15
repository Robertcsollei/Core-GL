#pragma once
#include <terrakit/core/Mesh.h>
#include <terrakit/core/Transform.h>
#include <terrakit/renderer/Material.h>

namespace terrakit::core {
struct Renderable
{
  Mesh* mesh = nullptr;
  terrakit::renderer::Material* material = nullptr;
  Transform transform{};

  Renderable() = default;
  Renderable(Mesh* m, terrakit::renderer::Material* mat, Transform xform)
    : mesh(m)
    , material(mat)
    , transform(std::move(xform))
  {
    if (mesh)
      transform.pivot = mesh->center;
  }
  Renderable(std::unique_ptr<Renderable> renderable)
    : mesh(renderable->mesh)
    , material(renderable->material)
    , transform(std::move(renderable->transform))
  {
    if (mesh)
      transform.pivot = mesh->center;
  }

  Renderable(const Renderable& renderable)
    : mesh(renderable.mesh)
    , material(renderable.material)
    , transform(renderable.transform)
  {
    if (mesh)
      transform.pivot = mesh->center;
  }
};
} // namespace terrakit::core