#include "TmMove.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

namespace test {


	TestMove::TestMove(glm::mat4* proj, glm::mat4* view)
		: m_Proj(proj), m_View(view)
	{
	}
	 
	TestMove::~TestMove()
	{
	}

	void TestMove::OnUpdate(float deltaTime)
	{
		// Update logic can be added here if needed
	}

	void TestMove::OnRender(Renderer* renderer)
	{
	}

    void TestMove::OnImGuiRender() {
        ImGuiIO& io = ImGui::GetIO();
        float dt = io.DeltaTime;

        ImGui::SliderFloat("Speed", &m_Speed, 10.f, 2000.f);

        // Up
        ImGui::Button("Move up");
        if (ImGui::IsItemActive()) {
            m_CamPos.y += m_Speed * dt;
        }

        // Left / Right on one line
        ImGui::Button("Move left");
        if (ImGui::IsItemActive()) {
            m_CamPos.x -= m_Speed * dt;
        }

        ImGui::SameLine();
        ImGui::Button("Move right");
        if (ImGui::IsItemActive()) {
            m_CamPos.x += m_Speed * dt;
        }

        // Down
        ImGui::Button("Move down");
        if (ImGui::IsItemActive()) {
            m_CamPos.y -= m_Speed * dt;
        }

        // Rebuild the view matrix from camera position.
        // View = translate(I, -cameraPos)
        *m_View = glm::translate(glm::mat4(1.0f), -m_CamPos);
    }
}