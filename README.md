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
├── Physics Module               # Coming soon: Real-time simulation systems (Currently part of Core)
├── Geometry Engine              # Coming soon: Ellipsoid math, mesh generation (Currently part of Core)
└── Platform Abstraction         # Coming soon: Cross-platform windowing, input
```

## Quick Start
**Requirements:** C++20, OpenGL 3.3+, CMake 3.20+, vcpkg

### Desktop Build (Windows/Linux/macOS)
```bash
git clone https://github.com/Robertcsollei/TerraKit.git
cd TerraKit

# Set VCPKG environment variable
export VCPKG_ROOT=/path/to/vcpkg  # Windows: set VCPKG_ROOT=C:\vcpkg

# Configure and build using presets
cmake --preset x64-release    # or x64-debug for debugging
cmake --build --preset build-x64-release

# Run example application
./out/build/x64-release/examples/satelliteTracker/SatelliteTracker
```

### WebAssembly Build
```bash
# Requires Emscripten SDK installed
source /path/to/emsdk/emsdk_env.sh  # Windows: emsdk_env.bat

cmake --preset wasm-release
cmake --build --preset build-wasm-release

# Serve the web build
cd out/build/wasm-release
python -m http.server 8000  # Navigate to localhost:8000
```

### OpenGL Version Configuration
TerraKit supports multiple OpenGL versions for different target platforms:

| OpenGL Version | Target Platform | Shader Version | Description |
|----------------|-----------------|----------------|-------------|
| **OpenGL 3.3 Core** | Desktop (Default) | `#version 330 core` | Maximum compatibility, older hardware |
| **OpenGL 4.3 Core** | Modern Desktop | `#version 430 core` | Compute shaders, enhanced features |
| **OpenGL 4.6 Core** | Latest Desktop | `#version 460 core` | Latest features, best performance |
| **OpenGL ES 3.0** | WebAssembly/Mobile | `#version 300 es` | Web browsers via WebGL2 |
| **OpenGL ES 3.2** | Modern Mobile | `#version 320 es` | Enhanced mobile capabilities |

**Custom OpenGL Configuration:**
```bash
# Configure specific OpenGL version
cmake --preset x64-debug -DOPENGL_VERSION=430 -DOPENGL_PROFILE=Core
cmake --preset wasm-debug -DOPENGL_VERSION=320 -DOPENGL_PROFILE=ES

# Available options:
# OPENGL_VERSION: 330, 430, 460, 300, 320
# OPENGL_PROFILE: Core, ES
```

**Note:** WebAssembly builds automatically default to OpenGL ES 3.0, while desktop builds default to OpenGL 3.3 Core for maximum compatibility.

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















