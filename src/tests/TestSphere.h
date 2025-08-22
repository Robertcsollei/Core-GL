#pragma once
#include "Shader.h"
#include "Test.h"
#include "Texture.h"
#include "entities/Material.h"
#include "entities/Mesh.h"
#include "entities/Renderable.h"

namespace test {

class TestSphere : public test::Test
{

public:
  TestSphere();
  ~TestSphere();

public:
  void OnUpdate(float deltaTime) override;
  void OnRender(Renderer* renderer) override;
  void OnImGuiRender(Camera& camera) override;

private:
  std::shared_ptr<Shader> m_Shader;
  std::shared_ptr<Texture> m_Texture;
  std::unique_ptr<Mesh> m_Mesh;
  std::unique_ptr<Material> m_Material;

  Renderable m_Sphere;

  float m_RotateDeg = 15.f;
  float m_RotateSpeed = glm::radians(10.f);
};
}