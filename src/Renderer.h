#pragma once
#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
void GLCheckError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer {
public:
	void Clear() const;
	void Draw(VertexArray& va, IndexBuffer& ib, Shader& shader) const;
	void DrawPoint(Shader& shader) const;
	void SetWireframeMode(bool enabled) const;
};