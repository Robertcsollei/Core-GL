<img src="https://github.com/Robertcsollei/TerraKit/blob/main/src/res/images/TerraKit.jpg" alt="TerraKit" style="width: 43px; height: 43px;">
# TerraKit: Real-time Satellite Visualization Engine

A high-performance 3D satellite tracking and visualization system built with modern C++/OpenGL, demonstrating advanced graphics programming and geospatial mathematics.

## 🚀 Demo Video
[![Watch Demo Video](https://github.com/Robertcsollei/TerraKit/blob/main/src/res/images/thumbnail.png)](https://player.vimeo.com/video/1117971664?h=8118259a26)

## Key Features
- **Real-time satellite tracking** - 14,000+ space objects at 60 FPS
- **Cross-platform renderer** - Native C++/OpenGL + Web via WebAssembly
- **Modular architecture** - Independent renderer, simulation, and networking layers
- **High-precision orbital mechanics** - SGP4 propagation with geodetic transformations
- **Interactive 3D globe** - Custom camera controls and object selection

## Technical Highlights
```cpp
// Modern OpenGL pipeline
✓ VAO/VBO/IBO abstractions with RAII
✓ Shader compilation with uniform caching  
✓ Instanced rendering for massive datasets
✓ Cross-platform input handling (SDL2)
✓ WebGL2 target via Emscripten
```

## Architecture
```
                    ┌─────────────────┐
                    │     Projects    │
                    │      (Apps)     │
                    └─────────────────┘
                             │
         ┌───────────────────┼───────────────────┐
         │                   │                   │
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│    Renderer     │ │   Networking    │ │       UI        │
│   (OpenGL)      │ │      (WIP)      │ │    (Future)     │
└─────────────────┘ └─────────────────┘ └─────────────────┘
         │                   │                   │
         └───────────────────┼───────────────────┘
                             │
                    ┌─────────────────┐
                    │      Core       │
                    │   (Foundation)  │
                    └─────────────────┘
```

## Quick Start
```bash
git clone https://github.com/your-username/TerraKit.git
cd TerraKit
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/TerraKit
```

**Requirements:** C++20, OpenGL 3.3+, CMake 3.20+

## Technologies
**Core:** C++20, OpenGL, GLSL, GLM, SDL2  
**Build:** CMake, Emscripten (for Web)  
**Libraries:** Dear ImGui, stb_image

## Development Focus
This project demonstrates:
- Modern graphics programming patterns
- Real-time performance optimization  
- Cross-platform development
- Geospatial coordinate system mathematics
- Modular software architecture

Built as a learning exercise to bridge 3D application development with low-level graphics programming.

---
**License:** Apache-2.0 | **Status:** Active Development








