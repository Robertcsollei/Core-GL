#pragma once
#include "Test.h"


namespace test {

class TestClearColor : public test::Test
{
public: 
	TestClearColor();
	~TestClearColor();

public:
	void OnUpdate(float deltaTime) override;
	void OnRender(Renderer* renderer) override;
	void OnImGuiRender() override;

	const std::string& GetName() const override { return "Clear Color"; }

private:
	float m_ClearColor[4];

};

}
