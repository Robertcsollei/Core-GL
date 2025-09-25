#include <terrakit/core/Logger.h>
#include <terrakit/renderer/Renderer.h>
#include <terrakit/renderer/Shader.h>

using namespace terrakit::renderer;

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
  m_RendererID = CreateShader(vertexSrc, fragmentSrc);
}

Shader::Shader(
  const std::string& vertexSrc,
  const std::string& fragmentSrc,
  std::vector<std::pair<std::string, unsigned int>>&& uniformBlocks)
  : m_UniformBlockCache(std::move(uniformBlocks))
{
  m_RendererID = CreateShader(vertexSrc, fragmentSrc);
}

Shader::Shader(const std::string& vertexSrc,
               const std::string& fragmentSrc,
               std::vector<std::pair<std::string, unsigned int>>& uniformBlocks)
  : m_UniformBlockCache(uniformBlocks)
{
  m_RendererID = CreateShader(vertexSrc, fragmentSrc);
}

Shader::~Shader()
{
  GLCall(glDeleteProgram(m_RendererID));
}

void
Shader::Bind() const
{
  GLCall(glUseProgram(m_RendererID));
}

void
Shader::Unbind() const
{
  GLCall(glUseProgram(0));
}

void
Shader::SetUniform4f(std::string_view name,
                     float v0,
                     float v1,
                     float v2,
                     float v3)
{

  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniform4f(loc, v0, v1, v2, v3));
}

void
Shader::SetUniform4f(std::string_view name, const glm::vec4& vec4)
{

  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w));
}

void
Shader::SetUniform3f(std::string_view name, float v0, float v1, float v2)
{

  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniform3f(loc, v0, v1, v2));
}

void
Shader::SetUniform3f(std::string_view name, const glm::vec3& vec3)
{

  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniform3f(loc, vec3.x, vec3.y, vec3.z));
}

void
Shader::SetUniform2f(std::string_view name, const glm::vec2& vec2)
{
  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniform2f(loc, vec2.x, vec2.y));
}

void
Shader::SetUniform1f(std::string_view name, float value)
{
  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniform1f(loc, value));
}

void
Shader::SetUniform1i(std::string_view name, int value)
{
  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniform1i(loc, value));
}

void
Shader::SetUniformMat4f(std::string_view name, const glm::mat4& matrix)
{
  const int loc = GetUniformLocation(name);
  if (loc != -1)
    GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]));
}

int
Shader::GetUniformLocation(std::string_view name) const
{

  if (auto it = m_UniformLocationCache.find(std::string{ name });
      it != m_UniformLocationCache.end())
    return it->second;

  GLCall(int location =
           glGetUniformLocation(m_RendererID, std::string{ name }.c_str()));
  TK_INFO(std::string("location of ") + std::string(name) +
          std::string(" loc:") + std::to_string(location));
  if (location == -1) {
    TK_ERROR(std::string("Warning: uniform '") + std::string(name) +
             std::string("' not found in shader program.\n"));
    return location;
  }

  // store even -1 to avoid repeated GL calls
  m_UniformLocationCache.emplace(std::string{ name }, location);
  return location;
}

unsigned int
Shader::CompileShader(unsigned int type, const std::string& source)
{
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  int ok = 0;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &ok));
  if (!ok) {
    int len = 0;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
    std::string log(len, '\0');
    GLCall(glGetShaderInfoLog(id, len, &len, log.data()));
    TK_ERROR(std::string("Failed to compile shader:\n") + log);
    GLCall(glDeleteShader(id));
    return 0;
  }
  return id;
}

unsigned int
Shader::CreateShader(const std::string& vsSrc, const std::string& fsSrc)
{
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vsSrc);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fsSrc);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));

  for (auto& uniformBlock : m_UniformBlockCache) {
    const std::string& name = uniformBlock.first;
    unsigned binding = uniformBlock.second;

    GLCall(GLuint blockIndex = glGetUniformBlockIndex(program, name.c_str()));
    if (blockIndex != GL_INVALID_INDEX) {
      GLCall(glUniformBlockBinding(program, blockIndex, binding));
    }
  }
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  return program;
}
