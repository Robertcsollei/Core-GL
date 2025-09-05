#pragma once
#include <core/entities/Renderable.h>
#include <core/geometry/Ellipsoid.h>

class AppContext;

class Globe
{
public:
  Globe(AppContext& ctx);
  ~Globe() = default;

  Ellipsoid& Geometry() { return m_Ellipsoid; }
  Renderable* RenderTask() { return &m_Renderable; }

private:
  Ellipsoid m_Ellipsoid;
  Renderable m_Renderable;
};