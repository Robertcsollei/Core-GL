#pragma once
#include <string>
#include <terrakit/core/libraries/MaterialLibrary.h>
#include <terrakit/core/libraries/MeshLibrary.h>
#include <terrakit/core/libraries/ShaderLibrary.h>
#include <terrakit/core/libraries/TextureLibrary.h>

namespace terrakit::core {
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
}