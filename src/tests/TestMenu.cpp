#include "TestMenu.h"
#include "Renderer.h"
#include <imgui/imgui.h>

#include "TestEllipsoid.h"
#include "TestSphere.h"

namespace test {

TestMenu::TestMenu(Renderer* renderer, Camera& camera)
  : m_CurrentTest(nullptr)
  , m_Tests()
  , m_Renderer(renderer)
  , m_Camera(camera)
{
  RegisterTest<TestSphere>("Sphere");
  RegisterTest<TestEllipsoid>("Ellipsoid");
}

TestMenu::~TestMenu()
{
  delete m_CurrentTest;
}

void
TestMenu::OnUpdate(float deltaTime)
{
  if (m_CurrentTest)
    m_CurrentTest->OnUpdate(deltaTime);
}

void
TestMenu::OnRender()
{
  if (m_CurrentTest)
    m_CurrentTest->OnRender(m_Renderer);
  else
    m_Renderer->Clear();
}

void
TestMenu::OnImGuiRender()
{

  ImGui::Begin("Test Menu");

  if (m_CurrentTest) {
    if (ImGui::Button("Back"))
      changeTest(nullptr);
  } else {
    for (auto& test : m_Tests) {
      if (ImGui::Button(test.first.c_str())) {
        changeTest(test.second());
      }
    }
  }

  if (m_CurrentTest) {
    m_CurrentTest->OnImGuiRender(m_Camera);
  }

  static bool wireframeMode = false;
  if (ImGui::Checkbox("Wireframe mode: ", &wireframeMode)) {
    m_Renderer->SetWireframeMode(wireframeMode);
  }

  ImGui::End();
}

void
TestMenu::changeTest(test::Test* test)
{
  delete m_CurrentTest;
  m_CurrentTest = test;
}

}