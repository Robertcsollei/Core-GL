#pragma once
#include <glm/glm.hpp>
#include <terrakit/core/config/ResourceHandle.h>
#include <terrakit/core/libraries/ShaderLibrary.h>
#include <terrakit/renderer/Renderer.h>
#include <vector>

namespace terrakit::core {
class Renderable;
class Mesh;
class Camera;
}

namespace terrakit::core::config {

class RenderContext
{
public:
  RenderContext();

  ShaderHandle LoadShader(const std::string& name,
                          const std::string& vertexSrc,
                          const std::string& fragmentSrc);
  ShaderHandle GetShader(const std::string& name);

  void SetupDefaultShaders();

  void UpdateScreenSize(float width, float height);
  glm::vec2 GetScreenSize() const { return m_ScreenSize; }
  void SetWireframeMode(bool enabled) { m_WireframeMode = enabled; };
  void SetClearColor(const glm::vec4& color) { m_ClearColor = color; };

  // Rendering interface - resolves shaders and delegates to renderer
  void Clear() const;
  void BeginFrame(const terrakit::core::Camera& cam);
  void EndFrame() const;
  void Submit(terrakit::core::Renderable* r);
  void SubmitPoint(terrakit::core::Renderable* r);
  void SubmitLine(terrakit::core::Renderable* r);
  void SubmitPointsInstanced(
    terrakit::core::Renderable* r,
    const std::vector<terrakit::core::Mesh::PointVertex>& positions);

  glm::vec4& clearColor() { return m_ClearColor; }

private:
  ShaderLibrary m_Shaders;
  terrakit::renderer::Renderer m_Renderer;
  glm::vec2 m_ScreenSize{ 0.0f, 0.0f };
  glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.3f, 1.f };
  bool m_WireframeMode = false;
};

} // namespace terrakit::core::config