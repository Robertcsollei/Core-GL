#include "core/shaders/Atmosphere.shader.h"
#include "core/shaders/Line.shader.h"
#include "core/shaders/Object.shader.h"
#include "core/shaders/Point.shader.h"
#include "core/shaders/ShaderNames.h"
#include <stdexcept>
#include <terrakit/core/libraries/ShaderLibrary.h>

using namespace terrakit::renderer;

namespace terrakit::core {

ShaderLibrary::ShaderLibrary()
{
  RegisterSource(ShaderNames::Object3dShader,
                 Shaders::Object3d::vertex.c_str(),
                 Shaders::Object3d::fragment.c_str(),
                 { { "Camera", 0 } });

  RegisterSource(ShaderNames::PointShader,
                 Shaders::Point::vertex.c_str(),
                 Shaders::Point::fragment.c_str());

  RegisterSource(ShaderNames::LineShader,
                 Shaders::Line::vertex.c_str(),
                 Shaders::Line::fragment.c_str());

  RegisterSource(ShaderNames::AtmosphereShader,
                 Shaders::Atmosphere::vertex.c_str(),
                 Shaders::Atmosphere::fragment.c_str());
}

std::shared_ptr<Shader>
ShaderLibrary::Get(const std::string& name)
{
  auto it = m_Cache.find(name);
  if (it != m_Cache.end())
    return it->second;

  auto src = m_Sources.find(name);
  if (src == m_Sources.end())
    throw std::runtime_error("ShaderLibrary: unknown shader " + name);

  auto sh =
    std::make_shared<Shader>(src->second.vs, src->second.fs, src->second.ubos);
  m_Cache[name] = sh;
  return sh;
}

void
ShaderLibrary::RegisterSource(
  const std::string& name,
  const std::string& vs,
  const std::string& fs,
  std::vector<std::pair<std::string, unsigned>> ubos)
{
  m_Sources[name] = { vs, fs, std::move(ubos) };
}
} // namespace terrakit::core