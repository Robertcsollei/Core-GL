# TerraKit: 3D GIS Rendering Engine
<img src="https://github.com/Robertcsollei/TerraKit/blob/main/assets/images/TerraKit.png" alt="TerraKit" style="width: 43px; height: 43px;">

A modular 3D rendering engine for geospatial visualization and analysis. Demonstrates real-time physics simulation, coordinate system transformations, and high-performance rendering of large datasets.

## 🚀 Demo Video
[![Watch Demo Video](https://github.com/Robertcsollei/TerraKit/blob/main/assets/images/thumbnail.png)](https://vimeo.com/1119403473?fl=pl&fe=sh)

## Core Capabilities
- **High-performance 3D rendering** - Instanced rendering for massive datasets (12k+ objects at 144+ FPS)
- **Geospatial coordinate systems** - Geodetic transformations, ellipsoid mathematics
- **Physics simulation** - Modular double precision physics engine with real-time calculations
- **Interactive 3D controls** - Orbit, pan, zoom with precision camera system
- **Cross-platform architecture** - Native C++ with WebAssembly support planned
- **Scalability**: Modular design supports diverse visualization needs

## Technical Architecture
```
Applications Layer
├── Demo: Space Object Tracker    # Example: orbital mechanics simulation
└── [Future: Terrain, Weather, etc.]

Engine
├── Renderer (OpenGL 3.3+)       # Modern graphics pipeline, WebGL compatible
├── Core                         # Runtime, Scene, core orchestrator
├── Network                      # HTTP client for data fetching
├── Physics Module               # Coming soon: Real-time simulation systems
├── Geometry Engine              # Coming soon: Ellipsoid math, mesh generation
└── Platform Abstraction         # Coming soon: Cross-platform windowing, input
```

## Quick Start
**Requirements:** C++20, OpenGL 3.3+, CMake 3.20+, vcpkg

```bash
git clone https://github.com/Robertcsollei/TerraKit.git
cd TerraKit

# Set VCPKG environment variable
export VCPKG_ROOT=/path/to/vcpkg  # Windows: set VCPKG_ROOT=C:\vcpkg

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Run example application
./build/examples/satelliteTracker/SatelliteTracker
```

## Core Technologies
- **Languages**: C++20, GLSL
- **Graphics**: OpenGL 3.3, SDL2, GLM
- **Build System**: CMake, vcpkg
- **Libraries**: glad, stb, cpr, Dear ImGui

## Development Philosophy
This engine explores fundamental challenges in 3D GIS:
- **Coordinate precision** - Handling Earth-scale coordinates without floating-point errors
- **Performance at scale** - Rendering thousands of dynamic objects smoothly
- **Modular architecture** - Physics, rendering, and coordinate systems as independent modules
- **Cross-platform deployment** - Native performance with web deployment capability

## Planned Extensions
- **WebAssembly build** - Browser deployment for wider accessibility
- **Terrain rendering** - Digital elevation models, texture streaming
- **Raster data pipeline** - Satellite imagery, weather data visualization
- **Additional coordinate systems** - UTM, State Plane, custom projections
- **Level of detail (LOD)** - Dynamic geometry optimization

---
**License:** Apache-2.0  | **Status:** Active Development














