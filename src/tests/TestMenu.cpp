#include "TestMenu.h"
#include "Renderer.h"
#include <imgui/imgui.h>

#include "TestSphere.h"

namespace test {

TestMenu::TestMenu()
  : m_CurrentTest(nullptr)
  , m_Tests()
{
  RegisterTest<TestSphere>("Sphere");
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
TestMenu::OnRender(Renderer* renderer)
{
  if (m_CurrentTest)
    m_CurrentTest->OnRender(renderer);
  else
    renderer->Clear();
}

void
TestMenu::OnImGuiRender(Camera& camera)
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
    m_CurrentTest->OnImGuiRender(camera);
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