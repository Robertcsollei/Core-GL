#pragma once
#include <glm/glm.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace terrakit::renderer {
class Shader
{
public:
  Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
  Shader(const std::string& vertexSrc,
         const std::string& fragmentSrc,
         std::vector<std::pair<std::string, unsigned int>>&& uniformBlocks);
  Shader(const std::string& vertexSrc,
         const std::string& fragmentSrc,
         std::vector<std::pair<std::string, unsigned int>>& uniformBlocks);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void SetUniform4f(std::string_view name,
                    float v0,
                    float v1,
                    float v2,
                    float v3);
  void SetUniform4f(std::string_view name, glm::vec4 vec4);

  void SetUniform3f(std::string_view name, float v0, float v1, float v2);
  void SetUniform3f(std::string_view name, glm::vec3 vec3);

  void SetUniform1f(std::string_view name, float value);
  void SetUniform1i(std::string_view name, int value);

  void SetUniformMat4f(std::string_view name, const glm::mat4& matrix);

  unsigned int GetRendererID() const { return m_RendererID; }

private:
  unsigned int m_RendererID = 0;
  mutable std::unordered_map<std::string, int> m_UniformLocationCache;
  mutable std::vector<std::pair<std::string, unsigned int>> m_UniformBlockCache;

  unsigned int CreateShader(const std::string& vertexSrc,
                            const std::string& fragmentSrc);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  int GetUniformLocation(std::string_view name) const;
};
} // namespace terrakit::renderer