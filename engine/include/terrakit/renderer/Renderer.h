#pragma once

#include <glad/glad.h>
#include <terrakit/core/Mesh.h>
#include <terrakit/renderer/IndexBuffer.h>
#include <terrakit/renderer/Shader.h>
#include <terrakit/renderer/VertexArray.h>

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    __debugbreak();
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
    terrakit::core::Mesh* mesh,
    Shader* shader,
    const std::vector<terrakit::core::Mesh::PointVertex>& positions);
  void SetWireframeMode(bool enabled) { m_WireframeMode = enabled; };

private:
  GLuint m_CameraUBO = 0;
  bool m_WireframeMode = false;

private:
  void SetPolygoneMode() const;
};
} // namespace terrakit::renderer