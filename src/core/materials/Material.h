#pragma once
#include "../Shader.h"
#include "../Texture.h"
#include <glad/glad.h>
#include <memory>
#include <string>
#include <vector>

class Material
{
public:
  struct State
  {
    bool depthTest = true;
    bool depthWrite = true;
    bool blending = false;
    GLenum blendSrc = GL_ONE;
    GLenum blendDst = GL_ZERO;
    bool cull = true;
    GLenum cullFace = GL_BACK;
  };

  struct SamplerBinding
  {
    int unit = 0;
    std::shared_ptr<Texture> texture;
    std::string uniform = "u_Texture";
  };

  std::shared_ptr<Shader> shader;
  std::vector<SamplerBinding> samplers;
  Material::State state{};

public:
  explicit Material(std::shared_ptr<Shader> s)
    : shader(std::move(s))
  {
  }

public:
  void addTexture(int unit,
                  std::shared_ptr<Texture> t,
                  std::string uniform = "u_Texture");
  void applyState() const;
  void bindTextures() const;
};