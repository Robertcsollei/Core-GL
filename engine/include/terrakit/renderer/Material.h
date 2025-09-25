#pragma once
#include <terrakit/renderer/OpenGL.h>
#include <memory>
#include <string>
#include <terrakit/renderer/Shader.h>
#include <terrakit/renderer/Texture.h>
#include <variant>
#include <vector>

namespace terrakit::renderer {
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

  struct UniformBinding
  {
    enum class Type
    {
      Float,
      Vec2,
      Vec3,
      Vec4,
      Int,
    };
    Type type;
    void* ptr;        // raw pointer to external data
    std::string name; // uniform name in shader
  };

  std::shared_ptr<Shader> shader;
  std::vector<SamplerBinding> samplers;
  Material::State state{};

public:
  explicit Material(const std::string& id,
                    const std::string& n,
                    std::shared_ptr<Shader> s)
    : uuid(id)
    , name(n)
    , shader(std::move(s))
  {
  }

  std::string name;
  const std::string uuid;
  std::vector<UniformBinding> uniforms;

public:
  void addTexture(int unit,
                  std::shared_ptr<Texture> t,
                  std::string uniform = "u_Texture");
  void applyState() const;
  void applyUniforms() const;
  void bindTextures() const;
};
} // namespace terrakit::core