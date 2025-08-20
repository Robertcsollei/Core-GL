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


namespace test {

	class TestColorQuad : public test::Test
	{
	
	public:
        TestColorQuad();
		~TestColorQuad();
	
	public:
		void OnUpdate(float deltaTime) override;
		void OnRender(Renderer* renderer) override;
		void OnImGuiRender() override;

		std::string GetName() const override { return "Color Triangle"; }

	private:
		std::vector<Vertex> m_Vertecies;
		const unsigned int m_Indices[6];

		float m_Rotation = 0.0f;
		glm::vec3 m_Pivot;
		glm::vec3 m_Translation;
		glm::vec3 m_Scale;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexBufferLayout> m_VBOLayout;
		std::unique_ptr<IndexBuffer> m_IBO;

		std::unique_ptr<Shader> m_PointShader;
		glm::vec4 m_PivotColor;

		glm::mat4 m_Proj, m_View;
	
	};
}