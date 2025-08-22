#pragma once
#include "../Renderer.h"
#include "../entities/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

namespace test {

class Test
{
public:
  Test() = default;
  virtual ~Test() = default;

public:
  virtual void OnUpdate(float deltaTime) = 0;
  virtual void OnRender(Renderer* renderer) = 0;
  virtual void OnImGuiRender(Camera& camera) = 0;
};
}