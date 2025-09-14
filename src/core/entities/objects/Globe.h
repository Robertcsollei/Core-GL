#pragma once
#include <core/entities/Renderable.h>
#include <core/geometry/Ellipsoid.h>

class AppContext;

class Globe
{
public:
  Globe(AppContext& ctx);
  ~Globe() = default;

  const Ellipsoid& Geometry() const noexcept { return m_Ellipsoid; }
  Renderable* RenderTask() { return &m_Renderable; }
  Renderable* AtmosphereTask() { return &m_Atmosphere; }

private:
  Ellipsoid m_Ellipsoid;
  Renderable m_Renderable;
  Renderable m_Atmosphere;

  glm::vec3 m_AtmosphereColor;
};