#pragma once
#include <memory>
#include <string>
#include <terrakit/renderer/Shader.h>
#include <unordered_map>
#include <vector>

namespace terrakit::core {
class ShaderLibrary
{
public:
  ShaderLibrary();
  ~ShaderLibrary() = default;

public:
  std::shared_ptr<terrakit::renderer::Shader> Get(const std::string& name);

  std::shared_ptr<terrakit::renderer::Shader> Load(const std::string& name,
                                                   const std::string& vs,
                                                   const std::string& fs,
                                                   std::vector<std::pair<std::string, unsigned>> ubos = {});


private:
  struct Src
  {
    std::string vs, fs;
    std::vector<std::pair<std::string, unsigned int>> ubos;
  };
  std::unordered_map<std::string, Src> m_Sources;
  std::unordered_map<std::string, std::shared_ptr<terrakit::renderer::Shader>>
    m_Cache;
};
} // namespace terrakit::core