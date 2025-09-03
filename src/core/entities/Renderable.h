#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "core/materials/Material.h"

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
      transform.m_Pivot = mesh->center;
  }
};