#include <GLES3/gl3.h>
#include <memory>
#include <terrakit/platform/OpenGLLoader.h>

namespace terrakit::platform {

class EmscriptenOpenGLLoader : public OpenGLLoader
{
public:
  bool LoadGL() override
  {
    // Emscripten provides OpenGL ES functions automatically
    // No loader needed
    return true;
  }

  const char* GetVersionString() override
  {
    return (const char*)glGetString(GL_VERSION);
  }
};

std::unique_ptr<OpenGLLoader>
OpenGLLoader::Create()
{
  return std::make_unique<EmscriptenOpenGLLoader>();
}

}