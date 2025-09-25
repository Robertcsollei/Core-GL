#pragma once
#include <memory>

namespace terrakit::platform {

/**
 * @brief Platform-specific OpenGL loader and context management
 *
 * Desktop: Uses GLAD with SDL loader
 * Emscripten: Uses built-in WebGL/OpenGL ES support
 */
class OpenGLLoader
{
public:
  virtual ~OpenGLLoader() = default;

  /**
   * @brief Load OpenGL functions for the platform
   * @return True if successful, false otherwise
   */
  virtual bool LoadGL() = 0;

  /**
   * @brief Get the appropriate OpenGL version string for logging
   * @return Version string
   */
  virtual const char* GetVersionString() = 0;

  /**
   * @brief Create platform-appropriate OpenGL loader
   * @return Unique pointer to loader implementation
   */
  static std::unique_ptr<OpenGLLoader> Create();
};

}