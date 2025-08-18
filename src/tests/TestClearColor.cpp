#include "TestClearColor.h"
#include "Renderer.h"
#include <imgui/imgui.h>

namespace test {


	TestClearColor::TestClearColor()
		: m_ClearColor { 0.2f, 0.3f, 0.8f, 1.0f },
		  m_ClearColor2{ 0.8f, 0.2f, 0.3f, 1.0f },
		  m_ClearColor3{ 0.2f, 0.8f, 0.2f, 1.0f },
		  m_ClearColor4{ 0.2f, 0.4f, 0.2f, 1.0f }
	{

	}

	TestClearColor::~TestClearColor()
	{
	}
	


	void TestClearColor::OnRender() 
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	}


	void TestClearColor::OnImGuiRender()
	{
		ImGui::Begin("Clear Color");
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::ColorEdit4("Clear Color 2", m_ClearColor2);
		ImGui::ColorEdit4("Clear Color 3", m_ClearColor3);
		ImGui::ColorEdit4("Clear Color 4", m_ClearColor4);
		
		ImGui::End();
	}
}