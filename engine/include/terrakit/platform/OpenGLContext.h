#pragma once

#include <SDL.h>
#include <string>

namespace terrakit::platform {

/**
 * @brief OpenGL context configuration based on compile-time settings
 *
 * Configures OpenGL version, profile, and shader compatibility
 * based on CMake-defined TERRAKIT_OPENGL_* definitions
 */
class OpenGLContext
{
public:
  enum class Profile
  {
    Core,
    ES
  };

  enum class Version
  {
    GL330, // OpenGL 3.3
    GL430, // OpenGL 4.3
    GL460, // OpenGL 4.6
    ES300, // OpenGL ES 3.0
    ES320  // OpenGL ES 3.2
  };

  /**
   * @brief Get the compile-time configured OpenGL profile
   * @return OpenGL profile (Core or ES)
   */
  static constexpr Profile GetProfile()
  {
#ifdef TERRAKIT_OPENGL_PROFILE_ES
    return Profile::ES;
#else
    return Profile::Core;
#endif
  }

  /**
   * @brief Get the compile-time configured OpenGL version
   * @return OpenGL version enum
   */
  static constexpr Version GetVersion()
  {
#ifdef TERRAKIT_OPENGL_VERSION_300
    return Version::ES300;
#elif defined(TERRAKIT_OPENGL_VERSION_320)
    return Version::ES320;
#elif defined(TERRAKIT_OPENGL_VERSION_330)
    return Version::GL330;
#elif defined(TERRAKIT_OPENGL_VERSION_430)
    return Version::GL430;
#elif defined(TERRAKIT_OPENGL_VERSION_460)
    return Version::GL460;
#else
// Default fallback
#ifdef TERRAKIT_OPENGL_PROFILE_ES
    return Version::ES300;
#else
    return Version::GL330;
#endif
#endif
  }

  /**
   * @brief Get major version number for SDL context creation
   * @return Major version (3, 4, etc.)
   */
  static constexpr int GetMajorVersion()
  {
#ifdef TERRAKIT_OPENGL_VERSION_330
    return 3;
#elif defined(TERRAKIT_OPENGL_VERSION_430) ||                                  \
  defined(TERRAKIT_OPENGL_VERSION_460)
    return 4;
#elif defined(TERRAKIT_OPENGL_VERSION_300) ||                                  \
  defined(TERRAKIT_OPENGL_VERSION_320)
    return 3;
#else
#ifdef TERRAKIT_OPENGL_PROFILE_ES
    return 3;
#else
    return 3;
#endif
#endif
  }

  /**
   * @brief Get minor version number for SDL context creation
   * @return Minor version (3, 0, 6, etc.)
   */
  static constexpr int GetMinorVersion()
  {
#ifdef TERRAKIT_OPENGL_VERSION_330
    return 3;
#elif defined(TERRAKIT_OPENGL_VERSION_430)
    return 3;
#elif defined(TERRAKIT_OPENGL_VERSION_460)
    return 6;
#elif defined(TERRAKIT_OPENGL_VERSION_300)
    return 0;
#elif defined(TERRAKIT_OPENGL_VERSION_320)
    return 2;
#else
#ifdef TERRAKIT_OPENGL_PROFILE_ES
    return 0;
#else
    return 3;
#endif
#endif
  }

  /**
   * @brief Get SDL profile mask for context creation
   * @return SDL_GL_CONTEXT_PROFILE_CORE or SDL_GL_CONTEXT_PROFILE_ES
   */
  static constexpr int GetSDLProfileMask()
  {
#ifdef TERRAKIT_OPENGL_PROFILE_ES
    return SDL_GL_CONTEXT_PROFILE_ES;
#else
    return SDL_GL_CONTEXT_PROFILE_CORE;
#endif
  }

  /**
   * @brief Generate appropriate shader version string (compile-time)
   * @return Shader version header (e.g., "#version 330 core\n")
   *
   * Note: Using highp precision for ES to prevent Z-fighting on mobile
   */
  static constexpr const char* GetShaderVersion()
  {
#ifdef TERRAKIT_OPENGL_VERSION_330
    return "#version 330 core\n";
#elif defined(TERRAKIT_OPENGL_VERSION_430)
    return "#version 430 core\n";
#elif defined(TERRAKIT_OPENGL_VERSION_460)
    return "#version 460 core\n";
#elif defined(TERRAKIT_OPENGL_VERSION_300)
    return "#version 300 es\nprecision highp float;\n";
#elif defined(TERRAKIT_OPENGL_VERSION_320)
    return "#version 320 es\nprecision highp float;\n";
#else
#ifdef TERRAKIT_OPENGL_PROFILE_ES
    return "#version 300 es\nprecision highp float;\n";
#else
    return "#version 330 core\n";
#endif
#endif
  }

  /**
   * @brief Get ImGui OpenGL version string
   * @return Version string for ImGui_ImplOpenGL3_Init()
   */
  static constexpr const char* GetImGuiVersion()
  {
#ifdef TERRAKIT_OPENGL_VERSION_330
    return "#version 130";
#elif defined(TERRAKIT_OPENGL_VERSION_430) ||                                  \
  defined(TERRAKIT_OPENGL_VERSION_460)
    return "#version 150";
#elif defined(TERRAKIT_OPENGL_VERSION_300) ||                                  \
  defined(TERRAKIT_OPENGL_VERSION_320)
    return "#version 300 es";
#else
#ifdef TERRAKIT_OPENGL_PROFILE_ES
    return "#version 300 es";
#else
    return "#version 130";
#endif
#endif
  }
};

}