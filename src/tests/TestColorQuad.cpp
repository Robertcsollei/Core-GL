#include "TestColorQuad.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../data/gradient.shader.h"
#include "../data/point.shader.h"
#include <imgui/imgui.h>

namespace test {


	TestColorQuad::TestColorQuad()
		: m_Vertecies{
			Vertex{ glm::vec3(50.f, 150.f, 0.f), glm::vec4(1.f, 0.f, 0.f, 1.f) },
			Vertex{ glm::vec3(150.f, 50.f, 0.f), glm::vec4(0.f , 1.f, 0.f, 1.f) },
			Vertex{ glm::vec3(250.f, 150.f, 0.f), glm::vec4(0.f , 0.f, 1.f, 1.f) },
			Vertex{ glm::vec3(150.f, 250.f, 0.f), glm::vec4(1.f , 1.f, 0.f, 1.f) }
		},
		m_Pivot(),
		m_PivotColor(1.f, 1.f, 1.f, 1.f),
		m_Translation(0.0f, 0.0f, 0.0f),
		m_Scale(1.f, 1.f, 1.f),
	    m_Indices{ 0, 1, 2, 2, 3, 0 },
		m_Proj(glm::ortho(0.0f, 1240.f, 0.0f, 860.f, -1.0f, 1.0f)),
		m_View(glm::mat4(1.0f))
	{
		glEnable(GL_PROGRAM_POINT_SIZE);
		m_Shader = std::make_unique<Shader>(Shaders::Gradient::vertex, Shaders::Gradient::fragment);
		m_VAO = std::make_unique<VertexArray>();
	 	m_VBO = std::make_unique<VertexBuffer>(m_Vertecies.data(), m_Vertecies.size() * sizeof(Vertex), GL_DYNAMIC_DRAW);
		m_VBOLayout = std::make_unique<VertexBufferLayout>();
		m_IBO = std::make_unique<IndexBuffer>(m_Indices, 6);

		m_PointShader = std::make_unique<Shader>(Shaders::Point::vertex, Shaders::Point::fragment);

		Vertex::AppendLayout(m_VBOLayout.get());
		
		m_VAO->addVertexBuffer(*m_VBO, *m_VBOLayout);

		glm::vec3 sum(0.0f);
		for (const auto& v : m_Vertecies) sum += v.Position;

		if (!m_Vertecies.empty())
			m_Pivot = sum * (1.0f / static_cast<float>(m_Vertecies.size()));

		m_Shader->Bind();

		m_VBO->Unbind();
		m_VAO->Unbind();
		m_IBO->Unbind();
		m_Shader->Unbind();

		m_PointShader->Bind();
		m_PointShader->SetUniform1f("u_PointSize", 10.0f);
		m_PointShader->SetUniform3f("u_Pivot", m_Pivot.x, m_Pivot.y, m_Pivot.z);
		m_PointShader->SetUniform4f("u_Color", m_PivotColor.x, m_PivotColor.y, m_PivotColor.z, m_PivotColor.w);
		m_PointShader->Unbind();
		
	}

	TestColorQuad::~TestColorQuad()
	{
		m_VBO->Unbind();
		m_VAO->Unbind();
		m_IBO->Unbind();
		m_Shader->Unbind();
		m_PointShader->Unbind();
	}

	void TestColorQuad::OnUpdate(float deltaTime)
	{
	}

	void TestColorQuad::OnRender(Renderer* renderer)
	{
		glm::mat4 M0(1.0f);                  // base model (translate/scale if you have any)

		m_Shader->Bind();  

		glm::mat4 model = M0;
		model = glm::translate(model, m_Translation);
		model = glm::scale(model, m_Scale); 
		model = glm::translate(model, m_Pivot);
		model = glm::rotate(model, m_Rotation, glm::vec3(0, 0, 1)); // around Z 
		model = glm::translate(model, -m_Pivot);


		glm::mat4 vp = m_Proj * m_View;
		glm::mat4 mvp = vp * model;

		m_Shader->SetUniformMat4f("u_MVP", mvp); 
		renderer->Draw(*m_VAO, *m_IBO, *m_Shader); 
		m_Shader->Unbind();

		m_PointShader->Bind();

		glm::mat4 pivot = M0;
		pivot = glm::translate(pivot, m_Translation);
		pivot = glm::scale(pivot, m_Scale); 
		glm::vec3 worldPivot = glm::vec3(pivot * glm::vec4(m_Pivot, 1.0f));
		
		m_PointShader->SetUniformMat4f("u_MVP", vp);
		m_PointShader->SetUniform3f("u_Pivot", worldPivot.x, worldPivot.y, worldPivot.z); 
		renderer->DrawPoint(*m_PointShader);

	}

	void TestColorQuad::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translate", &m_Translation.x, -3000.0f, 3000.0f);
		ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 100.0f);
		ImGui::SliderFloat3("Scale", &m_Scale.x, 0.1f, 10.0f);

		if (ImGui::ColorEdit4("Pivot Color", &m_PivotColor.x)) {
			m_PointShader->Bind();
			m_PointShader->SetUniform4f("u_Color", m_PivotColor.x, m_PivotColor.y, m_PivotColor.z, m_PivotColor.w); 
			m_PointShader->Unbind(); 
		}

		static bool enablePivot = false;
		ImGui::Checkbox("Enable Pivot", &enablePivot);
		if (enablePivot) {
			ImGui::SliderFloat3("Pivot", &m_Pivot.x, 0.0f, 1240.0f);
		}
		
	}

}