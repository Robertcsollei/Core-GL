#include "Shader.h"
#include "Renderer.h"
#include <iostream>

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const 
{ 
    GLCall(glUseProgram(m_RendererID)); 
}

void Shader::Unbind() const 
{ 
    GLCall(glUseProgram(0)); 
}

void Shader::SetUniform4f(std::string_view name, float v0, float v1, float v2, float v3) 
{

    const int loc = GetUniformLocation(name);
    if (loc != -1) GLCall(glUniform4f(loc, v0, v1, v2, v3));
}

void Shader::SetUniform1f(std::string_view name, float value)
{
    const int loc = GetUniformLocation(name);
    if (loc != -1) GLCall(glUniform1f(loc, value));
}

void Shader::SetUniform1i(std::string_view name, int value)
{
    const int loc = GetUniformLocation(name);
    if (loc != -1) GLCall(glUniform1i(loc, value));
}

void Shader::SetUniformMat4f(std::string_view name, glm::mat4& matrix)
{
    const int loc = GetUniformLocation(name);
    if (loc != -1) GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(std::string_view name) const 
{

    if (auto it = m_UniformLocationCache.find(std::string{ name }); it != m_UniformLocationCache.end())
        return it->second;

    GLCall(int location = glGetUniformLocation(m_RendererID, std::string{ name }.c_str()));
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' not found in shader program.\n";
    }

    // store even -1 to avoid repeated GL calls
    m_UniformLocationCache.emplace(std::string{ name }, location);
    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) 
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
        std::cerr << "Failed to compile shader:\n" << log << '\n';
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vsSrc, const std::string& fsSrc) 
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vsSrc);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fsSrc);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}
