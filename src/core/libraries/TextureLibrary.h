#pragma once
#include "Texture.h"
#include <memory>
#include <string>
#include <unordered_map>

class TextureLibrary
{
public:
  std::shared_ptr<Texture> load2D(const std::string& path); // caches by path
private:
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_Cache;
};
