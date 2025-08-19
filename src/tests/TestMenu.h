#pragma once
#include "Test.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>


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

		std::string GetName() const override { return "Test Menu"; }
		
		template<typename T>
		void RegisterTest(const std::string& name) 
		{
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		test::Test* m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	
	private:
		void changeTest(test::Test* test);
	};
}