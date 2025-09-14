#pragma once
#include <core/entities/Mesh.h>
#include <core/entities/Transform.h>
#include <core/materials/Material.h>

struct Renderable
{
  Mesh* mesh = nullptr;
  Material* material = nullptr;
  Transform transform{};

  Renderable() = default;
  Renderable(Mesh* m, Material* mat, Transform xform)
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