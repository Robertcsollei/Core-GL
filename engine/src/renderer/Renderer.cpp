#include <terrakit/core/Logger.h>
#include <terrakit/core/Mesh.h>
#include <terrakit/core/Renderable.h>
#include <terrakit/core/entities/Camera.h>
#include <terrakit/renderer/Renderer.h>

using namespace terrakit::core;
using namespace terrakit::renderer;

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
    TK_ERROR(std::string("OpenGL Error: ") + std::to_string(error));
  }
}
bool
GLLogCall(const char* function, const char* file, int line)
{
  while (GLenum error = glGetError()) {
    TK_ERROR(std::string("OpenGL Error: ") + std::to_string(error) +
             " ;At: " + function + " " + file + ":" + std::to_string(line));

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

#ifndef TERRAKIT_EMSCRIPTEN
  // GL_PROGRAM_POINT_SIZE is not available in WebGL/OpenGL ES
  GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
#endif
}

void
Renderer::Clear(const glm::vec4& clearColor) const
{
  GLCall(glDepthMask(GL_TRUE));
  GLCall(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void
Renderer::BeginFrame(const Camera& cam, const glm::vec4& clearColor)
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

  Clear(clearColor);
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
Renderer::Submit(Renderable* r, const config::ShaderHandle& shader)
{
  if (!r->mesh || !r->material || !shader)
    return;

  // 1) Program
  shader->Bind();

  // 2) Per-object uniforms
  static const glm::vec3 lightDirection(0.5f, 1.0f, 0.2f);
  const glm::mat4 model = r->transform.world();
  shader->SetUniformMat4f("u_Model", model); // View/Proj come from Camera UBO
  // shader->SetUniform3f("u_LightDir", lightDirection);

  // 3) Material state + textures
  r->material->applyState(shader);
  r->material->bindTextures(shader);

  // 4) Geometry
  r->mesh->vao.Bind();
  glDrawElements(
    GL_TRIANGLES, r->mesh->ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void
Renderer::SubmitPoint(Renderable* r, const config::ShaderHandle& shader)
{
  if (!r->mesh || !r->material || !shader)
    return;

  shader->Bind();

  shader->SetUniformMat4f("u_Model", r->transform.world());

  r->material->applyState(shader);
  r->mesh->vao.Bind();
  glDrawArrays(GL_POINTS, 0, 1);
}

void
Renderer::SubmitLine(Renderable* r, const config::ShaderHandle& shader)
{
  if (!r->mesh || !r->material || !shader)
    return;

  shader->Bind();

  shader->SetUniformMat4f("u_Model", r->transform.world());

  r->material->applyState(shader);
  r->mesh->vao.Bind();
  glDrawArrays(GL_LINE_STRIP, 0, r->mesh->vbo.size());
}

void
Renderer::SubmitPointsInstanced(Renderable* r,
                                const config::ShaderHandle& shader,
                                const std::vector<Mesh::PointVertex>& vertecies)
{
  if (!r->mesh || !r->material || !shader)
    return;

  shader->Bind();

  r->mesh->vao.Bind();

  auto& vbo = r->mesh->instanceVbo;
  const GLsizeiptr bytes = vertecies.size() * sizeof(Mesh::PointVertex);

  vbo.Bind();

  GLCall(glBufferData(GL_ARRAY_BUFFER, bytes, nullptr, GL_STREAM_DRAW));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, vertecies.data()));
  GLCall(glDrawArraysInstanced(GL_POINTS, 0, 1, (GLsizei)vertecies.size()));
}

void
Renderer::SetPolygoneMode() const
{
  /* if (m_WireframeMode) {
     GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
   } else {
     GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
   }*/
}
