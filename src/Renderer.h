#pragma once
#include "./entities/Camera.h"
#include "./entities/Renderable.h"
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

class Renderer
{
public:
  Renderer();

public:
  void Clear() const;
  void BeginFrame(const Camera& cam);
  void EndFrame() const;
  void Submit(const Renderable& r);
  void Draw(VertexArray& va, IndexBuffer& ib, Shader& shader) const;
  void DrawPoint(Shader& shader) const;
  void SetWireframeMode(bool enabled) const;

private:
  GLuint m_CameraUBO = 0;
};