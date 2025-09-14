#include <core/libraries/ShaderLibrary.h>
#include <core/shaders/Atmosphere.shader.h>
#include <core/shaders/Color.shader.h>
#include <core/shaders/Object.shader.h>
#include <core/shaders/Point.shader.h>
#include <core/shaders/ShaderNames.h>
#include <stdexcept>

ShaderLibrary::ShaderLibrary()
{
  registerSource(ShaderNames::Object3dShader,
                 Shaders::Object3d::vertex,
                 Shaders::Object3d::fragment,
                 { { "Camera", 0 } });

  registerSource(
    ShaderNames::PointShader, Shaders::Point::vertex, Shaders::Point::fragment);

  registerSource(
    ShaderNames::ColorShader, Shaders::Color::vertex, Shaders::Color::fragment);

  registerSource(ShaderNames::AtmosphereShader,
                 Shaders::Atmosphere::vertex,
                 Shaders::Atmosphere::fragment);
}

std::shared_ptr<Shader>
ShaderLibrary::get(const std::string& name)
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
ShaderLibrary::registerSource(
  const std::string& name,
  const std::string& vs,
  const std::string& fs,
  std::vector<std::pair<std::string, unsigned>> ubos)
{
  m_Sources[name] = { vs, fs, std::move(ubos) };
}
