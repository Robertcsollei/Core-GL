#include "Renderer.h"
#include "core/entities/Renderable.h"
#include "core/entities/camera/Camera.h"
#include <core/entities/Mesh.h>
#include <iostream>

void
GLClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}

void
GLCheckError()
{
  while (GLenum error = glGetError()) {
    std::cout << "OpenGL Error: " << error << std::endl;
  }
}
bool
GLLogCall(const char* function, const char* file, int line)
{
  while (GLenum error = glGetError()) {
    std::cout << "OpenGL Error: " << error << " ;At: " << function << " "
              << file << ":" << line << std::endl;
    return false;
  }

  return true;
}

Renderer::Renderer()
{
  GLCall(glGenBuffers(1, &m_CameraUBO));
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO));
  GLCall(glBufferData(
    GL_UNIFORM_BUFFER, sizeof(Camera::UBO), nullptr, GL_DYNAMIC_DRAW));
  GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraUBO)); // binding = 0
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

  GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
}

void
Renderer::Clear() const
{
  GLCall(glDepthMask(GL_TRUE));
  GLCall(glClearColor(0.1f, 0.1f, 0.3f, 1.f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void
Renderer::BeginFrame(const Camera& cam)
{
  Camera::UBO u{};
  u.view = cam.view();
  u.proj = cam.proj();
  u.eye = glm::vec4(cam.position(), 1.0f);

  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO));
  GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Camera::UBO), &u));
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

  SetPolygoneMode();
  GLCall(glEnable(GL_DEPTH_TEST));
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  Clear();
}

void
Renderer::EndFrame() const
{
  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
  GLCall(glActiveTexture(GL_TEXTURE0));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  SetPolygoneMode();
  GLCall(glDisable(GL_BLEND));
  GLCall(glEnable(GL_DEPTH_TEST));
  GLCall(glDepthMask(GL_TRUE));
}

void
Renderer::Submit(Renderable* r)
{
  if (!r->mesh || !r->material || !r->material->shader)
    return;

  // 1) Program
  auto* shader = r->material->shader.get();
  shader->Bind();

  // 2) Per-object uniforms
  static const glm::vec3 lightDirection(0.5f, 1.0f, 0.2f);
  const glm::mat4 model = r->transform.world();
  shader->SetUniformMat4f("u_Model", model); // View/Proj come from Camera UBO
  shader->SetUniform3f("u_LightDir", lightDirection);

  // 3) Material state + textures
  r->material->applyState();
  r->material->bindTextures(); // no-op for your current shader

  // 4) Geometry
  r->mesh->vao.Bind();
  glDrawElements(
    GL_TRIANGLES, r->mesh->ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void
Renderer::SubmitPoint(Renderable* r)
{
  if (!r->mesh || !r->material || !r->material->shader)
    return;
  auto* shader = r->material->shader.get();
  shader->Bind();

  shader->SetUniformMat4f("u_Model", r->transform.world());

  r->material->applyState();
  r->mesh->vao.Bind();
  glDrawArrays(GL_POINTS, 0, 1);
}

void
Renderer::SubmitPointsInstanced(Mesh* mesh,
                                Shader* shader,
                                const std::vector<glm::vec3>& positions)
{
  shader->Bind();
  mesh->vao.Bind();

  glBindBuffer(GL_ARRAY_BUFFER, mesh->instanceVbo.GetRendererID());
  glBufferSubData(
    GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());

  glDrawArraysInstanced(GL_POINTS, 0, 1, positions.size());
}

void
Renderer::SetPolygoneMode() const
{
  if (m_WireframeMode) {
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
  } else {
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  }
}
