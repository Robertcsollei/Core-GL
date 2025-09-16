#include <terrakit/renderer/Material.h>

namespace terrakit::renderer {
void
Material::addTexture(int unit, std::shared_ptr<Texture> t, std::string uniform)
{
  samplers.push_back({ unit, std::move(t), std::move(uniform) });
}

void
Material::applyState() const
{
  state.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  glDepthMask(state.depthWrite ? GL_TRUE : GL_FALSE);

  if (state.blending) {
    glEnable(GL_BLEND);
    glBlendFunc(state.blendSrc, state.blendDst);
  } else {
    glDisable(GL_BLEND);
  }

  if (state.cull) {
    glEnable(GL_CULL_FACE);
    glCullFace(state.cullFace);
  } else {
    glDisable(GL_CULL_FACE);
  }
  applyUniforms();
}

void
Material::applyUniforms() const
{

  for (auto& u : uniforms) {
    switch (u.type) {
      case UniformBinding::Type::Float:
        shader->SetUniform1f(u.name, *static_cast<float*>(u.ptr));
        break;
      case UniformBinding::Type::Vec2:
        shader->SetUniform2f(u.name, *static_cast<glm::vec2*>(u.ptr));
        break;
      case UniformBinding::Type::Vec3:
        shader->SetUniform3f(u.name, *static_cast<glm::vec3*>(u.ptr));
        break;
      case UniformBinding::Type::Vec4:
        shader->SetUniform4f(u.name, *static_cast<glm::vec4*>(u.ptr));
        break;
      case UniformBinding::Type::Int:
        shader->SetUniform1i(u.name, *static_cast<int*>(u.ptr));
        break;
    }
  }
}

void
Material::bindTextures() const
{
  if (!shader)
    return;
  for (const auto& s : samplers) {
    if (!s.texture)
      continue;
    glActiveTexture(GL_TEXTURE0 + s.unit);
    s.texture->Bind();
    shader->SetUniform1i(s.uniform.c_str(), s.unit);
  }
}

} // namespace terrakit::core