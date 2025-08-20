#include "TestImageShading.h"
#include <imgui/imgui.h>

#include "../data/texture.shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace test
{

	TestImageShading::TestImageShading()
		: m_Vertices(),
		m_Indices(),
		m_Proj(glm::ortho(0.0f, 1240.f, 0.0f, 860.f, -1.0f, 1.0f)),
		m_View(glm::mat4(1.0f)),
		m_Move(&m_Proj, &m_View)
	{
		auto quad1 = Test::CreateRect(350.f, 350.f, 650.f, 400.f, { 1.f, 0.f, 0.f, 1.f });
		auto quad2 = Test::CreateRect(600.f, 600.f, 200.f, 200.f, { 0.f, 1.f, 0.f, 1.f });

		m_Vertices.clear();
		m_Vertices.reserve(quad1.size() + quad2.size());
		m_Vertices.insert(m_Vertices.end(), quad1.begin(), quad1.end());
		m_Vertices.insert(m_Vertices.end(), quad2.begin(), quad2.end());
		m_Indices = Test::CreateRectIndices(m_Vertices.size());

		m_Shader = std::make_unique<Shader>(Shaders::Texture::vertex, Shaders::Texture::fragment);
		m_Texture = std::make_unique<Texture>("res/textures/lake.jpg");
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex), GL_DYNAMIC_DRAW);
		m_VBOLayout = std::make_unique<VertexBufferLayout>();
		m_IBO = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());

		Vertex::AppendLayout(m_VBOLayout.get());

		m_VAO->addVertexBuffer(*m_VBO, *m_VBOLayout);

		m_Shader->Bind();

		unsigned int textureSlot = 0;
		m_Texture->Bind(textureSlot);
		m_Shader->SetUniform1i("u_Texture", textureSlot);

		m_VBO->Unbind();
		m_VAO->Unbind();
		m_IBO->Unbind();
		m_Shader->Unbind();

	}
	TestImageShading::~TestImageShading()
	{
	}

	void TestImageShading::OnUpdate(float deltaTime)
	{
		// Update logic can be added here if needed
		// For now, we are not using deltaTime in this test
	}

	void TestImageShading::OnRender(Renderer* renderer)
	{
		m_Shader->Bind();
		glm::mat4 mvp = m_Proj * m_View; 
		m_Shader->SetUniformMat4f("u_MVP", mvp); 
		renderer->Draw(*m_VAO, *m_IBO, *m_Shader); 
	}

	void TestImageShading::OnImGuiRender()
	{
		m_Move.OnImGuiRender();
	}



}