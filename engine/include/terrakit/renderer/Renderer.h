#pragma once

#include <terrakit/core/Mesh.h>
#include <terrakit/core/config/ResourceHandle.h>
#include <terrakit/renderer/IndexBuffer.h>
#include <terrakit/renderer/OpenGL.h>
#include <terrakit/renderer/Shader.h>
#include <terrakit/renderer/VertexArray.h>

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#elif defined(__APPLE__)
#define DEBUG_BREAK() __builtin_trap()
#elif defined(__linux__)
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#else
#define DEBUG_BREAK() ((void)0)
#endif

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    DEBUG_BREAK();
#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void
GLClearError();
void
GLCheckError();
bool
GLLogCall(const char* function, const char* file, int line);

namespace terrakit::core {
class Camera;
class Renderable;
}

namespace terrakit::renderer {
class Renderer
{

public:
  Renderer();

public:
  void Clear(const glm::vec4& clearColor) const;
  void BeginFrame(const terrakit::core::Camera& cam,
                  const glm::vec4& clearColor);
  void EndFrame() const;
  void Submit(terrakit::core::Renderable* r,
              const terrakit::core::config::ShaderHandle& shader);
  void SubmitPoint(terrakit::core::Renderable* r,
                   const terrakit::core::config::ShaderHandle& shader);
  void SubmitLine(terrakit::core::Renderable* r,
                  const terrakit::core::config::ShaderHandle& shader);
  void SubmitPointsInstanced(
    terrakit::core::Renderable* r,
    const terrakit::core::config::ShaderHandle& shader,
    const std::vector<terrakit::core::Mesh::PointVertex>& positions);

private:
  GLuint m_CameraUBO = 0;

private:
  void SetPolygoneMode() const;
};
} // namespace terrakit::renderer