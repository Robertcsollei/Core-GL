#pragma once
#include <string>
#include <terrakit/core/libraries/MaterialLibrary.h>
#include <terrakit/core/libraries/MeshLibrary.h>
#include <terrakit/core/libraries/ShaderLibrary.h>
#include <terrakit/core/libraries/TextureLibrary.h>

namespace terrakit::core {
struct SatOptions
{
  float nearDistance = 20.f;
  float farDistance = 200.0f;
  float minSize = 2.f;
  float maxSize = 10.0f;

  uint32_t defaultColor = 0xFFFFFFFF;
  uint32_t hoverColor = 0xFF0000FF;
  uint32_t selectColor = 0xFF00FF00;

  float orbitThickness = 2.0f;
};

struct AppContext
{
  const std::string& title = "TerraKit Lite";

  // TODO: prevent accidental overriding. Quickfix for now.
  float width;
  float height;
  glm::vec2 screenSize = { 0, 0 };

  bool pointerLock = false;

  SatOptions satOptions;

  ShaderLibrary shaders;
  TextureLibrary textures;
  MeshLibrary meshes;
  MaterialLibrary materials;

  void UpdateSize(float w, float h)
  {
    width = w;
    height = h;
    screenSize.x = w;
    screenSize.y = h;
  }

  AppContext(float w, float h) { UpdateSize(w, h); }
};
}
