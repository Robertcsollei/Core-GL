#include "Material.h"

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
