#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glad/glad.h>

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
class Mesh;

class Renderer
{
public:
  Renderer();

public:
  void Clear() const;
  void BeginFrame(const Camera& cam);
  void EndFrame() const;
  void Submit(Renderable* r);
  void SubmitPoint(Renderable* r);
  void SubmitPointsInstanced(Mesh* mesh,
                             Shader* shader,
                             const std::vector<glm::vec3>& positions);
  void SetWireframeMode(bool enabled) { m_WireframeMode = enabled; };

private:
  GLuint m_CameraUBO = 0;
  bool m_WireframeMode = false;

private:
  void SetPolygoneMode() const;
};