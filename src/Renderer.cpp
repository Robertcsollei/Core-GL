#include "Renderer.h"
#include <iostream>




void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}


void GLCheckError()
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error: " << error << std::endl;
    }
}
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error: " << error
            << " ;At: " << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}



void Renderer::Clear() const {
    GLCall(glClearColor(0.1f, 0.1f, 0.3f, 1.f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(VertexArray& va, IndexBuffer& ib, Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); 
}

void Renderer::DrawPoint(Shader& shader) const
{
	shader.Bind();
	GLCall(glDrawArrays(GL_POINTS, 0, 1););
}

void Renderer::SetWireframeMode(bool enabled) const
{
    if (enabled)
    {
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
    else
    {
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}
}