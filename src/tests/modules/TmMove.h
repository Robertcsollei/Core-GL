#pragma once
#include "../Test.h"


namespace test {
	class TestMove : public Test 
	{
	public:
		TestMove(glm::mat4* Proj, glm::mat4* View);
		~TestMove();

		void OnUpdate(float deltaTime) override;
		void OnRender(Renderer* renderer) override;
		void OnImGuiRender() override;

		std::string GetName() const override { return "Test Move"; }

	private:
		glm::mat4 *m_Proj, *m_View;
		glm::vec3 m_CamPos = glm::vec3(0.0f); 
		float m_Speed = 650.0f;
	};
}