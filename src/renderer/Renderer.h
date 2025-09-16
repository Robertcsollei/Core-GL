#pragma once

#include <core/entities/Mesh.h>
#include <glad/glad.h>
#include <renderer/IndexBuffer.h>
#include <renderer/Shader.h>
#include <renderer/VertexArray.h>

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

class Camera;
class Renderable;

class Renderer
{
public:
  Renderer();

public:
  void Clear() const;
  void BeginFrame(const Camera& cam);
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