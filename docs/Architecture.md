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

## Design Philosophies

### GPU-First Memory Architecture

TerraKit uses a **GPU-first** approach where vertex data is uploaded to GPU memory immediately and CPU copies are discarded. This design choice optimizes for large-scale GIS visualization.

**Implementation:**
- Mesh constructors upload vertices to VBO/IBO during creation
- CPU vertex arrays are destroyed after upload
- Only metadata (AABB bounds, center point) retained on CPU
- VAO/VBO/IBO handles provide GPU access for rendering

**Benefits:**
- **Memory efficiency**: Eliminates duplicate CPU/GPU storage for large datasets
- **Scalability**: Enables 14,000+ satellite visualization at 60 FPS
- **Performance**: Reduces memory pressure and improves cache locality
- **Platform consistency**: Same memory profile on desktop and WebAssembly

**Workarounds for CPU-side operations:**
- **Selection/raycasting**: Uses screen-space projection + 2D pixel distance instead of mesh raycasting
- **Collision detection**: Analytical sphere intersection (for globe occlusion) instead of triangle intersection
- **Dynamic content**: Separate entity-level data structures (satellite positions) for logic

**Trade-offs:**
- ❌ No runtime mesh editing or CPU-side collision queries
- ✅ Optimal for static/semi-static visualization workloads
- ✅ Fits TerraKit's GIS visualization focus perfectly

This architecture proves that complex UI interactions (hover, selection, occlusion) can work efficiently without CPU vertex storage through clever algorithmic alternatives.

**Future: Server-Side Query Architecture**
For operations requiring complex vertex analysis, TerraKit will leverage **server-side computation**:
- **Spatial queries**: Complex geometric searches performed server-side with full vertex data
- **Dynamic LOD**: Server generates meshes based on view parameters
- **Collision analysis**: Advanced intersection queries computed remotely
- **Data processing**: Filtering, aggregation, and analysis on server infrastructure

This **thin-client/thick-server** model validates the GPU-first approach by moving computational complexity to where resources are abundant, keeping the client optimized purely for high-performance visualization.

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