#pragma once

#include <glad/glad.h>
#include <terrakit/core/Mesh.h>
#include <terrakit/renderer/IndexBuffer.h>
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
  void Clear() const;
  void BeginFrame(const terrakit::core::Camera& cam);
  void EndFrame() const;
  void Submit(terrakit::core::Renderable* r);
  void SubmitPoint(terrakit::core::Renderable* r);
  void SubmitLine(terrakit::core::Renderable* r);
  void SubmitPointsInstanced(
    terrakit::core::Renderable* r,
    const std::vector<terrakit::core::Mesh::PointVertex>& positions);
  void SetWireframeMode(bool enabled) { m_WireframeMode = enabled; };
  void SetClearColor(const glm::vec4& color) { m_ClearColor = color; };

  glm::vec4& clearColor() { return m_ClearColor; }

private:
  GLuint m_CameraUBO = 0;
  glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.3f, 1.f };
  bool m_WireframeMode = false;

private:
  void SetPolygoneMode() const;
};
} // namespace terrakit::renderer