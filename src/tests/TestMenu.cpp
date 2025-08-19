#include "TestMenu.h"
#include "Renderer.h"
#include <imgui/imgui.h>

#include "TestClearColor.h"
#include "TestImagePlacement.h"

namespace test 
{

	TestMenu::TestMenu() 
		: m_CurrentTest(nullptr), 
		  m_CurrentTestName(GetName())
	{

	}

	TestMenu::~TestMenu()
	{
		delete m_CurrentTest;
	}

	void TestMenu::OnUpdate(float deltaTime)
	{
		if (m_CurrentTest)
			m_CurrentTest->OnUpdate(deltaTime);
	}

	void TestMenu::OnRender(Renderer* renderer)
	{
		if (m_CurrentTest)
			m_CurrentTest->OnRender(renderer);
		else
			renderer->Clear();
	}

	void TestMenu::OnImGuiRender()
	{

		ImGui::Begin(m_CurrentTestName.c_str());

		if (m_CurrentTest)
		{
			if (ImGui::Button("Close"))
				changeTest(nullptr);
		}
		else
		{
			if (ImGui::Button("Clear Color"))
				changeTest(new TestClearColor());
		    if (ImGui::Button("Image Placement"))
				changeTest(new TestImagePlacement());
		
		}

		if(m_CurrentTest) 
			m_CurrentTest->OnImGuiRender();

		ImGui::End();
	}

	void TestMenu::changeTest(test::Test* test)
	{
		delete m_CurrentTest;
		m_CurrentTest = test;
		if (m_CurrentTest)
			m_CurrentTestName = m_CurrentTest->GetName();
		else
			m_CurrentTestName = GetName();
	}


}