#pragma once
#include "Shader.h"
#include "Test.h"
#include "Texture.h"
#include "core/Geodetic3D.h"
#include "core/entities/Mesh.h"
#include "core/entities/Renderable.h"
#include "core/geometry/Ellipsoid.h"
#include "core/materials/Material.h"

namespace test {

class TestEllipsoid : public test::Test
{

public:
  TestEllipsoid();
  ~TestEllipsoid();

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

  std::unique_ptr<Mesh> m_MarkerMesh;
  Ellipsoid m_Ellipsoid;
  glm::vec2 m_LatLonAdeg{ 48.0f, 17.0f };
  glm::vec2 m_LatLonBdeg{ 0.0f, 0.0f };
  float m_HeightA{ 0.0f };
  float m_HeightB{ 0.0f };
  int m_CurveSamples{ 64 };
  bool m_ShowNormal{ true };

  Renderable m_MarkerA, m_MarkerB, m_NormalTip;
  std::vector<Renderable> m_CurveMarkers;

  // same center as your generated sphere
  glm::vec3 m_Center{ 0.f, 0.f, -800.f };

  float m_RotateSpeed = 0.0f;
};
}