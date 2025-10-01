#include <stdexcept>
#include <terrakit/core/libraries/ShaderLibrary.h>

using namespace terrakit::renderer;

namespace terrakit::core {

ShaderLibrary::ShaderLibrary()
{
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

std::shared_ptr<Shader>
ShaderLibrary::Load(const std::string& name,
                    const std::string& vs,
                    const std::string& fs,
                    std::vector<std::pair<std::string, unsigned>> ubos)
{
  m_Sources[name] = { vs, fs, std::move(ubos) };
  return Get(name);
}

} // namespace terrakit::core