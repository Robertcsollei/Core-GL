#include "core/shaders/Atmosphere.shader.h"
#include "core/shaders/Line.shader.h"
#include "core/shaders/Object.shader.h"
#include "core/shaders/Point.shader.h"
#include "core/shaders/ShaderNames.h"
#include <terrakit/core/Mesh.h>
#include <terrakit/core/Renderable.h>
#include <terrakit/core/config/RenderContext.h>
#include <terrakit/core/entities/Camera.h>

namespace terrakit::core::config {

RenderContext::RenderContext()
  : m_Renderer()
{
  SetupDefaultShaders();
}

ShaderHandle
RenderContext::LoadShader(const std::string& name,
                          const std::string& vertexSrc,
                          const std::string& fragmentSrc)
{
  auto shader = m_Shaders.Load(name, vertexSrc, fragmentSrc);
  if (shader) {
    return ShaderHandle(shader);
  }
  return ShaderHandle();
}

ShaderHandle
RenderContext::GetShader(const std::string& name)
{
  auto shader = m_Shaders.Get(name);
  if (shader) {
    return ShaderHandle(shader);
  }
  return ShaderHandle();
}

void
RenderContext::SetupDefaultShaders()
{
  m_Shaders.Load(ShaderNames::Object3dShader,
                 Shaders::Object3d::vertex.c_str(),
                 Shaders::Object3d::fragment.c_str(),
                 { { "Camera", 0 } });

  m_Shaders.Load(ShaderNames::PointShader,
                 Shaders::Point::vertex.c_str(),
                 Shaders::Point::fragment.c_str());

  m_Shaders.Load(ShaderNames::LineShader,
                 Shaders::Line::vertex.c_str(),
                 Shaders::Line::fragment.c_str());

  m_Shaders.Load(ShaderNames::AtmosphereShader,
                 Shaders::Atmosphere::vertex.c_str(),
                 Shaders::Atmosphere::fragment.c_str());
}

void
RenderContext::UpdateScreenSize(float width, float height)
{
  m_ScreenSize.x = width;
  m_ScreenSize.y = height;
}

void
RenderContext::Clear() const
{
  m_Renderer.Clear(m_ClearColor);
}

void
RenderContext::BeginFrame(const Camera& cam)
{
  m_Renderer.BeginFrame(cam, m_ClearColor);
}

void
RenderContext::EndFrame() const
{
  m_Renderer.EndFrame();
}

void
RenderContext::Submit(Renderable* r)
{
  if (!r->mesh || !r->material || r->material->shaderName.length() == 0)
    return;

  auto shader = GetShader(r->material->shaderName);
  if (!shader)
    return;

  m_Renderer.Submit(r, shader);
}

void
RenderContext::SubmitPoint(Renderable* r)
{
  if (!r->mesh || !r->material || r->material->shaderName.length() == 0)
    return;

  auto shader = GetShader(r->material->shaderName);
  if (!shader)
    return;

  m_Renderer.SubmitPoint(r, shader);
}

void
RenderContext::SubmitLine(Renderable* r)
{
  if (!r->mesh || !r->material || r->material->shaderName.length() == 0)
    return;

  auto shader = GetShader(r->material->shaderName);
  if (!shader)
    return;

  m_Renderer.SubmitLine(r, shader);
}

void
RenderContext::SubmitPointsInstanced(
  Renderable* r,
  const std::vector<Mesh::PointVertex>& positions)
{
  if (!r->mesh || !r->material || r->material->shaderName.length() == 0)
    return;

  auto shader = GetShader(r->material->shaderName);
  if (!shader)
    return;

  m_Renderer.SubmitPointsInstanced(r, shader, positions);
}

} // namespace terrakit::core::config