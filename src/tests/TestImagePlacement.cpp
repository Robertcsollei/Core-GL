#include "TestImagePlacement.h"
#include <imgui/imgui.h>

#include "../data/basic.shader.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"

#include <glm/gtc/matrix_transform.hpp>


namespace test {

	TestImagePlacement::TestImagePlacement() 
		: m_Positions{
			 -50.f, -50.f, 0.0f, 0.0f, // 0
			  50.f, -50.f, 1.0f, 0.0f, // 1
			  50.f,  50.f, 1.0f, 1.0f, // 2
			 -50.f,  50.f, 0.0f, 1.0f  // 3
		  },
		  m_Indices{0, 1, 2, 2, 3, 0},
		  m_Shader(Shaders::vertex, Shaders::fragment),
		  m_Texture("res/textures/test.png"),
		  m_VAO(),
		  m_VBO(m_Positions, 4 * 4 * sizeof(float)),
		  m_VBOLayout(),
		  m_IBO(m_Indices, 6),
		  m_Proj(glm::ortho(0.0f, 1240.f, 0.0f, 860.f, -1.0f, 1.0f)),
		  m_View(glm::mat4(1.0f)),
		  m_TranslationA(200, 200, 0),
		  m_TranslationB(400, 200, 0)
	{

		m_VBOLayout.Push<float>(2);
		m_VBOLayout.Push<float>(2);

		m_VAO.addVertexBuffer(m_VBO, m_VBOLayout);


		m_Shader.Bind();

		unsigned int textureSlot = 0;
		m_Texture.Bind(textureSlot);
		m_Shader.SetUniform1i("u_Texture", textureSlot);

		m_VBO.Unbind();
		m_VAO.Unbind();
		m_IBO.Unbind();
		m_Shader.Unbind();

		
	}

	TestImagePlacement::~TestImagePlacement()
	{
		m_Shader.Unbind();
		m_VAO.Unbind();
		m_IBO.Unbind();
		m_Texture.Unbind();
		m_VBO.Unbind();
	}

	void TestImagePlacement::OnUpdate(float deltaTime)
	{
	}

	void TestImagePlacement::OnRender(Renderer* renderer)
	{
		m_Shader.Bind();
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader.SetUniformMat4f("u_MVP", mvp);
			renderer->Draw(m_VAO, m_IBO, m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader.SetUniformMat4f("u_MVP", mvp);
			renderer->Draw(m_VAO, m_IBO, m_Shader);
		}
		
	}

	void TestImagePlacement::OnImGuiRender()
	{
		
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 860.f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 860.f);

	}
}