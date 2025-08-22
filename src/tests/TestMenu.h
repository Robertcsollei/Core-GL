#pragma once
#include "../entities/Camera.h"
#include "Test.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace test {

class TestMenu : public test::Test
{
public:
  TestMenu();
  ~TestMenu();

public:
  void OnUpdate(float deltaTime) override;
  void OnRender(Renderer* renderer) override;
  void OnImGuiRender(Camera& camera) override;

  template<typename T>
  void RegisterTest(const std::string& name)
  {
    m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
  }

private:
  test::Test* m_CurrentTest;
  std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

private:
  void changeTest(test::Test* test);
};
}