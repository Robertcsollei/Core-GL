#pragma once

#include "Test.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../IndexBuffer.h"
#include <glm/glm.hpp>

namespace test {

	class TestImagePlacement : public test::Test
	{
	public:
		TestImagePlacement();
		~TestImagePlacement();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender(Renderer* renderer) override;
		void OnImGuiRender() override;

		const std::string& GetName() const override { return "Image Placement"; }
	private:
			const float m_Positions[16];
			const unsigned int m_Indices[6];
			Shader m_Shader;
			Texture m_Texture;
			VertexArray m_VAO;
			VertexBuffer m_VBO;
			VertexBufferLayout m_VBOLayout;
			IndexBuffer m_IBO;
			glm::mat4 m_Proj;
			glm::mat4 m_View;
			glm::vec3 m_TranslationA;
			glm::vec3 m_TranslationB;
			
};
}