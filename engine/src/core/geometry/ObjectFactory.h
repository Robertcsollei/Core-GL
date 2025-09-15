#pragma once
#include <terrakit/core/AppContext.h>
#include <terrakit/core/Renderable.h>
#include <terrakit/core/SceneState.h>
#include <terrakit/core/geometry/Ellipsoid.h>

#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace terrakit::core {

struct GlobeConfig
{
  int slices = 64;
  int stacks = 32;
  glm::vec3 center{ 0.f, 0.f, -800.f };
  // TODO: remove hardcoded asset path!!!
  std::string texturePath = "assets/textures/earth.jpg";
  glm::quat rotation{};
  Ellipsoid* ellipsoid;
};

namespace ObjectFactory {
std::unique_ptr<Renderable>
CreateGlobe(const GlobeConfig& cfg, AppContext& ctx);

std::unique_ptr<Renderable>
CreateSatellite(const glm::vec3 pos, AppContext& ctx);

std::unique_ptr<Renderable>
CreateOrbit(const std::string name,
            double dt,
            const glm::vec3 pos,
            SceneState& state,
            AppContext& ctx);
}
} //  namespace terrakit::core