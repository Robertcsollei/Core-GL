#pragma once
#include "core/entities/Renderable.h"
#include "core/geometry/Ellipsoid.h"
#include <core/AppContext.h>

#include <glm/glm.hpp>
#include <memory>
#include <string>

struct GlobeConfig
{
  float radius = 100.f;
  int slices = 64;
  int stacks = 32;
  glm::vec3 center{ 0.f, 0.f, -800.f };
  std::string texturePath = "res/textures/earth.jpg";
  glm::quat rotation{};
  Ellipsoid* ellipsoid;
};

namespace ObjectFactory {
std::unique_ptr<Renderable>
CreateGlobe(const GlobeConfig& cfg, AppContext& ctx);

std::unique_ptr<Renderable>
CreateSatellite(const glm::vec3 pos, AppContext& ctx);
}
