#pragma once
#include "Test.h"
#include "core/entities/camera/Camera.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace test {

class TestMenu
{
public:
  TestMenu(Renderer* renderer, Camera& camera);
  ~TestMenu();

public:
  void OnUpdate(float deltaTime);
  void OnRender();
  void OnImGuiRender();

  template<typename T>
  void RegisterTest(const std::string& name)
  {
    m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
  }

private:
  test::Test* m_CurrentTest;
  std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
  Renderer* m_Renderer;
  Camera& m_Camera;

private:
  void changeTest(test::Test* test);
};
}