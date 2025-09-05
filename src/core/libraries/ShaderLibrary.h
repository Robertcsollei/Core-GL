#pragma once
#include "Shader.h"
#include <memory>
#include <string>
#include <unordered_map>

class ShaderLibrary
{
public:
  ShaderLibrary();
  ~ShaderLibrary() = default;

public:
  std::shared_ptr<Shader> get(const std::string& name); // lazy-load/compile
  void registerSource(const std::string& name,
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
  std::unordered_map<std::string, std::shared_ptr<Shader>> m_Cache;
};
