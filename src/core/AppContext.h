#pragma once

#include <core/libraries/MaterialLibrary.h>
#include <core/libraries/MeshLibrary.h>
#include <core/libraries/ShaderLibrary.h>
#include <core/libraries/TextureLibrary.h>

struct AppContext
{
  const std::string& title = "TerraKit Lite";

  float width;
  float height;

  bool pointerLock = false;

  ShaderLibrary shaders;
  TextureLibrary textures;
  MeshLibrary meshes;
  MaterialLibrary materials;

  AppContext(float w, float h)
    : width(w)
    , height(h)
  {
  }
};