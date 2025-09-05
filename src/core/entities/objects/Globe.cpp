#include "Globe.h"
#include <core/AppContext.h>
#include <core/entities/objects/Globe.h>
#include <core/geometry/ObjectFactory.h>

Globe::Globe(AppContext& ctx)
  : m_Ellipsoid(glm::dvec3(1.0, 0.95, 1.0))
  , m_Renderable(ObjectFactory::CreateGlobe(
      {
        .center = glm::vec3(0.f, 0.f, -800.f),
        .ellipsoid = &m_Ellipsoid,
      },
      ctx))
{
}
