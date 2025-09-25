# TerraKit Architecture

## Current State: WIP Refactoring Required

TerraKit is currently a working prototype with tightly coupled components that need architectural refinement for better maintainability and cross-platform deployment.

## Vision

Transform TerraKit into a modular real-time GIS engine with clear separation of concerns, enabling easy testing, WASM deployment, and extensibility.

## Target Architecture

### Core Modules

**Physics Module**
- Orbital mechanics calculations (Keplerian elements, SGP4/SDP4)
- Time system management (epoch conversions, propagation)
- Mathematical utilities (coordinate transformations, numerical solvers)

**Geometry Engine**
- Mesh generation (ellipsoids, orbits, points)
- Spatial indexing and culling
- Instanced rendering optimizations
- Material and texture management

**Platform Abstraction**
- Build-time file selection instead of runtime conditionals
- Separate implementations: `src/platform/desktop/` vs `src/platform/emscripten/`
- Network layer (CPR for desktop, Fetch API for WebAssembly)
- File I/O abstraction (native filesystem vs virtual filesystem)
- Input handling through SDL2 (cross-platform compatibility layer)

## Current Implementation Status

**OpenGL Configuration System**
- Compile-time OpenGL version selection via CMake
- Supports OpenGL 3.3/4.3/4.6 Core and OpenGL ES 3.0/3.2
- `constexpr` shader version generation
- VS2022 IntelliSense compatibility


## Architectural TODO

### Phase 1: Decouple Core Systems
- [ ] Extract orbital mechanics into standalone Physics module
- [ ] Separate mesh generation from rendering concerns
- [ ] Create platform-agnostic interfaces for I/O operations

### Phase 2: Clean Interfaces
- [ ] Define clear APIs between modules with input validation
- [ ] Implement dependency injection for cross-module communication
- [ ] Add comprehensive error handling at module boundaries

### Phase 3: Platform Optimization
- [ ] Implement WASM-optimized data structures and algorithms
- [ ] Create platform-specific build configurations
- [ ] Add performance profiling hooks for bottleneck identification

### Phase 4: Extensibility
- [ ] Plugin architecture for extensibility
- [ ] Configurable rendering pipelines
- [ ] Modular UI system supporting different frameworks (Tauri React UI? sticking with ImGui?)

## Benefits of Modular Design

- **Testability**: Each module can be unit tested in isolation
- **Performance**: Platform-specific optimizations without code duplication
- **Maintainability**: Clear ownership and reduced coupling
- **Deployment**: Selective module loading for different use cases (web vs desktop)

*This architecture serves as our north star - implement incrementally while maintaining working prototype. Beware, the architectural goals can change slightly based on how the project evolves.*