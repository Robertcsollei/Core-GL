#pragma once
#include "Shader.h"
#include "Texture.h"
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
  explicit Material(const std::string& id,
                    const std::string& n,
                    std::shared_ptr<Shader> s)
    : uuid(std::move(id))
    , name(std::move(n))
    , shader(std::move(s))
  {
  }

  std::string name;
  const std::string uuid;

public:
  void addTexture(int unit,
                  std::shared_ptr<Texture> t,
                  std::string uniform = "u_Texture");
  void applyState() const;
  void bindTextures() const;
};