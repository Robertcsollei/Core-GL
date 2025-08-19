#pragma once

#include "Test.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../IndexBuffer.h"
#include <glm/glm.hpp>
#include <memory>

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

		std::string GetName() const override { return "Image Placement"; }
	private:
			const float m_Positions[16];
			const unsigned int m_Indices[6];
			std::unique_ptr<Shader> m_Shader;
			std::unique_ptr<Texture> m_Texture;
			std::unique_ptr<VertexArray> m_VAO;
			std::unique_ptr<VertexBuffer> m_VBO;
			std::unique_ptr<VertexBufferLayout> m_VBOLayout;
			std::unique_ptr<IndexBuffer> m_IBO;
			
			glm::mat4 m_Proj, m_View;
			glm::vec3 m_TranslationA, m_TranslationB;
			
};
}