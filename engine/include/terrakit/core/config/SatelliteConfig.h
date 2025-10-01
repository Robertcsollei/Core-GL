#pragma once
#include <cstdint>

namespace terrakit::core::config {

struct SatelliteConfig {
    float nearDistance = 20.0f;
    float farDistance = 200.0f;
    float minSize = 2.0f;
    float maxSize = 10.0f;

    uint32_t defaultColor = 0xFFFFFFFF;
    uint32_t hoverColor = 0xFF0000FF;
    uint32_t selectColor = 0xFF00FF00;

    float orbitThickness = 2.0f;
};

} // namespace terrakit::core::config