#include "core/geometry/ObjectFactory.h"
#include "core/materials/MaterialFactory.h"
#include <terrakit/core/AppContext.h>
#include <terrakit/core/entities/Globe.h>

using namespace terrakit::renderer;

namespace terrakit::core {
Globe::Globe(AppContext& ctx)
  : m_Ellipsoid(glm::dvec3(100., 95., 100.))
  , m_AtmosphereColor(0.4f, 0.6f, 1.0f)
  , m_Renderable(ObjectFactory::CreateGlobe(
      {
        .center = glm::vec3(0.f, 0.f, -800.f),
        .ellipsoid = &m_Ellipsoid,
      },
      ctx))
  , m_Atmosphere(m_Renderable)
{
  m_Atmosphere.transform.scale = glm::vec3(1.01f);
  auto material = ctx.materials.Add(
    MaterialFactory::CreateAtmosphere(ctx.shaders, ctx.textures));
  material->uniforms.push_back(
    { Material::UniformBinding::Type::Vec3, &m_AtmosphereColor, "u_Color" });
  m_Atmosphere.material = material;
}
} // namespace terrakit::core