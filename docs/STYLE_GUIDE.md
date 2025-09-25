# TerraKit C++ Style Guide

This document defines the coding standards and conventions for TerraKit to ensure consistency and maintainability across the codebase.

## Naming Conventions

### Private Member Variables
Use `m_` prefix followed by PascalCase:
```cpp
class Example {
private:
    int m_Width;
    std::string m_FilePath;
    std::unique_ptr<Renderer> m_Renderer;
};
```

### Methods
Use PascalCase for all methods:
```cpp
class Window {
public:
    void InitSDL();
    void BeginImGuiFrame();
    bool PollEvent(SDL_Event& e);
};
```

### Accessor Methods
Use camelCase for simple accessor methods that return member fields:
```cpp
class Texture {
private:
    int m_Width;
    unsigned int m_RendererID;

public:
    int width() const { return m_Width; }
    unsigned int rendererID() const { return m_RendererID; }
};
```

### Public Member Variables
Use camelCase for public fields and struct members:
```cpp
struct SatOptions {
    float nearDistance = 20.f;
    float farDistance = 200.0f;
    uint32_t defaultColor = 0xFFFFFFFF;
};
```

### Constants
Use UPPER_SNAKE_CASE for constants:
```cpp
constexpr double GM_EARTH = 3.986004418e14;  // m^3/s^2
constexpr double WGS84_A = 6378137.0;        // m (equatorial radius)
```

### Namespaces
Use lowercase with underscores:
```cpp
namespace terrakit::core {
namespace terrakit::renderer {
```

## String Handling

### String Concatenation with + Operator
When using the + operator with string literals, **ALL** string literals must be wrapped with `std::string()`:

```cpp
// Correct - ALL string literals wrapped
std::string errorMsg = std::string("Failed to load texture: ") + stbi_failure_reason() +
                      std::string(" | Path: ") + path +
                      std::string(" | Working directory: ") + workingDir;

// Incorrect - missing std::string() wrappers
std::string errorMsg = std::string("Failed to load texture: ") + stbi_failure_reason() +
                      " | Path: " + path +  // ERROR: bare string literal
                      " | Working directory: " + workingDir;  // ERROR: bare string literal
```

### Logging with String Concatenation
Apply the same rule when building strings for logging:
```cpp
// Correct
TK_ERROR(std::string("Cannot open file: ") + filePath + std::string(" | Reason: ") + error);

// Incorrect
TK_ERROR(std::string("Cannot open file: ") + filePath + " | Reason: " + error);
```

## Error Handling

### Logging
Use the TerraKit logging system instead of std::cout/std::cerr:
```cpp
TK_DEBUG("Debug message");
TK_INFO("Informational message");
TK_WARNING("Warning message");
TK_ERROR("Error message");
```

### Exception Handling
- Throw exceptions for unrecoverable errors
- Use descriptive error messages
- Log before throwing when appropriate

```cpp
if (!m_LocalBuffer) {
    std::string errorMsg = std::string("Failed to load texture: ") + stbi_failure_reason() +
                          std::string(" | Path: ") + path;
    TK_ERROR(errorMsg);
    throw std::runtime_error("Failed to load texture: " + path);
}
```

## Cross-Platform Compatibility

### Debug Breaks
Use the cross-platform DEBUG_BREAK() macro instead of platform-specific calls:
```cpp
// Correct
#define ASSERT(x) if (!(x)) DEBUG_BREAK();

// Incorrect
#define ASSERT(x) if (!(x)) __debugbreak();  // Windows-only
```

### File Paths
Use configurable asset paths from AppContext:
```cpp
// Correct
std::string iconPath = ctx.iconPath + "favicon.ico";

// Incorrect
std::string iconPath = "assets/images/favicon.ico";  // Hardcoded
```

## Documentation

### API Documentation
Use Doxygen-style comments for public APIs and complex algorithms:
```cpp
/**
 * @brief Computes satellite position in Earth-Centered Inertial (ECI) frame
 *
 * Implements classical orbital mechanics using Keplerian elements:
 * 1. Computes mean motion using Kepler's third law: n = sqrt(μ/a³)
 * 2. Solves Kepler's equation iteratively for eccentric anomaly
 *
 * @param t Time in seconds since Unix epoch
 * @param globe Reference globe for coordinate transformation
 * @return Position vector in ECI coordinates (world units)
 */
glm::dvec3 positionECI(double t, Globe& globe, double multiplier) const;
```

### Avoid Over-Documentation
Don't document obvious functionality:
```cpp
// Avoid this - obvious from signature
/**
 * @brief Gets the width
 * @return The width value
 */
int width() const { return m_Width; }

// Better - no documentation needed for simple accessors
int width() const { return m_Width; }
```

## Cross-Platform Architecture

### Platform Abstraction
TerraKit uses build-time file selection instead of runtime conditionals. Platform-specific implementations are organized in separate directories:

```
src/platform/
├── desktop/           # Windows/Linux/macOS implementations
│   ├── DesktopMainLoop.cpp
│   ├── DesktopOpenGLLoader.cpp
│   └── DesktopNetworkClient.cpp
├── emscripten/        # WebAssembly implementations
│   ├── EmscriptenMainLoop.cpp
│   ├── EmscriptenOpenGLLoader.cpp
│   └── EmscriptenNetworkClient.cpp
└── common/           # Shared platform code
    └── OpenGLContext.cpp
```

### OpenGL Configuration
Use compile-time OpenGL configuration instead of runtime detection:
```cpp
// Correct - compile-time configuration
static const std::string vertex =
  std::string(terrakit::platform::OpenGLContext::GetShaderVersion()) + R"(
  layout(location=0) in vec3 a_Position;
  // ... shader code
)";

// Incorrect - runtime conditionals (deprecated)
#ifdef __EMSCRIPTEN__
    std::string shaderVersion = "#version 300 es\n";
#else
    std::string shaderVersion = "#version 330 core\n";
#endif
```

### Platform-Specific Code
Avoid `#ifdef __EMSCRIPTEN__` in favor of platform-specific files:
```cpp
// Correct - separate platform implementations
// src/platform/desktop/DesktopLogger.cpp
void Logger::EnableConsoleOutput() {
    std::cout << message << std::endl;
}

// src/platform/emscripten/EmscriptenLogger.cpp
void Logger::EnableConsoleOutput() {
    emscripten_console_log(message.c_str());
}

// Incorrect - inline conditionals (avoid)
#ifdef __EMSCRIPTEN__
    emscripten_console_log(message.c_str());
#else
    std::cout << message << std::endl;
#endif
```

### CMake Platform Configuration
Platform selection is handled by CMake build system:
```cmake
# CMake automatically selects platform files
if(EMSCRIPTEN)
    set(PLATFORM_DIR "emscripten")
    set(PLATFORM_DEFINITIONS TERRAKIT_EMSCRIPTEN TERRAKIT_WEB)
else()
    set(PLATFORM_DIR "desktop")
    set(PLATFORM_DEFINITIONS TERRAKIT_DESKTOP)
endif()
```

## Code Organization

### Class Layout
Organize class members in this order:
1. Public types and enums
2. Public constructors/destructor
3. Public member variables (if any)
4. Public methods
5. Protected member variables
6. Protected methods
8. Private member variables
8. Private methods

### Method Organization
- Group related methods together
- Put constructors/destructors first
- Order methods logically (initialization → usage → cleanup)

## Performance Considerations

### Memory Management
- Use RAII principles
- Prefer smart pointers over raw pointers if possible
- Use move semantics where appropriate

```cpp
// Good - RAII with smart pointers
std::unique_ptr<Texture> texture = std::make_unique<Texture>(path);

// Good - move semantics
auto renderable = std::make_unique<Renderable>();
AddRenderable(std::move(renderable));
```

---

*This style guide is a living document. Update it as the codebase evolves and new patterns emerge.*