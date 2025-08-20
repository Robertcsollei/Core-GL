#pragma once
#include "Test.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../IndexBuffer.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "modules/TmMove.h"

namespace test {

class TestBatchRenderingColor : public test::Test
{
public:
	TestBatchRenderingColor();
	~TestBatchRenderingColor();

public:
	void OnUpdate(float deltaTime) override;
	void OnRender(Renderer* renderer) override;
	void OnImGuiRender() override;

	std::string GetName() const override { return "Batch Rendering Color"; }

private:
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexBufferLayout> m_VBOLayout;
	std::unique_ptr<IndexBuffer> m_IBO;

	glm::mat4 m_Proj, m_View;

	TestMove m_Move;
};
}