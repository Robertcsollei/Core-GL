#pragma once

#ifdef TERRAKIT_EMSCRIPTEN
// For Emscripten, use built-in OpenGL support (no loader needed)
#include <GLES3/gl3.h>
#define GL_GLEXT_PROTOTYPES 1
#else
// For desktop, use GLAD
#include <glad/glad.h>
#endif