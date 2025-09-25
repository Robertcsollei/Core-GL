#include <SDL.h>
#include <glad/glad.h>
#include <memory>
#include <terrakit/platform/OpenGLLoader.h>
#include <terrakit/renderer/OpenGL.h>

namespace terrakit::platform {

class DesktopOpenGLLoader : public OpenGLLoader
{
public:
  bool LoadGL() override
  {
    return gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
  }

  const char* GetVersionString() override
  {
    return (const char*)glGetString(GL_VERSION);
  }
};

std::unique_ptr<OpenGLLoader>
OpenGLLoader::Create()
{
  return std::make_unique<DesktopOpenGLLoader>();
}

}