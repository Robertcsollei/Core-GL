#pragma once
#include "Test.h"
#include <vector>
#include <memory>


namespace test
{

	class TestMenu : public test::Test
	{
	public:
		TestMenu();
		~TestMenu();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender(Renderer* renderer) override;
		void OnImGuiRender() override;

		const std::string& GetName() const override { return "Test Menu"; }

	private:
		test::Test* m_CurrentTest;
		std::string m_CurrentTestName;
	
	private:
		void changeTest(test::Test* test);
	};
}