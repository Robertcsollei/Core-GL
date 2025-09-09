#include "Globe.h"
#include <core/AppContext.h>
#include <core/entities/objects/Globe.h>
#include <core/geometry/ObjectFactory.h>

Globe::Globe(AppContext& ctx)
  : m_Ellipsoid(glm::dvec3(100., 95., 100.))
  , m_Renderable(ObjectFactory::CreateGlobe(
      {
        .center = glm::vec3(0.f, 0.f, -800.f),
        .ellipsoid = &m_Ellipsoid,
      },
      ctx))
{
}
