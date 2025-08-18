#pragma once
#include "Test.h"


namespace test {

class TestClearColor : public test::Test
{
public: 
	TestClearColor();
	~TestClearColor();

	void OnUpdate(float deltaTime) override {}
	void OnRender() override;
	void OnImGuiRender() override;


private:
	float m_ClearColor[4];
	float m_ClearColor2[4];
	float m_ClearColor3[4];
	float m_ClearColor4[4];

};

}
