#pragma once
#include <terrakit/core/Renderable.h>
#include <terrakit/core/geometry/Ellipsoid.h>

namespace terrakit::core {

class AppContext;

class Globe
{
public:
  Globe(AppContext& ctx);
  ~Globe() = default;

  const Ellipsoid& geometry() const noexcept { return m_Ellipsoid; }
  Renderable* renderTask() { return &m_Renderable; }
  Renderable* atmosphereTask() { return &m_Atmosphere; }

private:
  Ellipsoid m_Ellipsoid;
  Renderable m_Renderable;
  Renderable m_Atmosphere;

  glm::vec3 m_AtmosphereColor;
};
} // namespace terrakit::core