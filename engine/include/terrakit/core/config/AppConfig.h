#pragma once
#include <string>
#include <glm/glm.hpp>

namespace terrakit::core::config {

struct SatelliteOptions
{
  float nearDistance = 20.f;
  float farDistance = 200.0f;
  float minSize = 2.f;
  float maxSize = 10.0f;

  uint32_t defaultColor = 0xFFFFFFFF;
  uint32_t hoverColor = 0xFF0000FF;
  uint32_t selectColor = 0xFF00FF00;

  float orbitThickness = 2.0f;
};

class AppConfig
{
public:
  AppConfig(const std::string& assetsPath = "assets/")
    : assetsPath(assetsPath)
    , texturesPath(assetsPath + "textures/")
    , mockDataPath(assetsPath + "mock/")
    , iconPath(assetsPath + "images/")
  {
  }

  // Asset paths
  std::string assetsPath;
  std::string texturesPath;
  std::string mockDataPath;
  std::string iconPath;

  // Application configuration
  SatelliteOptions satelliteOptions;
  glm::vec2 screenSize{ 0.0f, 0.0f };
};

} // namespace terrakit::core::config