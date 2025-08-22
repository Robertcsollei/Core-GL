#include "TestSphere.h"
#include <glm/gtc/matrix_transform.hpp>

#include "TestUtils.h"
#include "data/cam.shader.h"
#include <imgui/imgui.h>

namespace test {

TestSphere::TestSphere()
{
  glm::vec3 center(0.f, 0.f, -800.f);
  // 1) Build sphere using your existing helper (into your "old" Vertex type)
  std::vector<Vertex> oldVerts;
  std::vector<uint32_t> indices;
  test::AppendSphere(oldVerts, indices, center, 100.f, 64, 32, glm::vec4(1.f));

  // 2) Convert to Mesh::Vertex (adjust mapping if your Vertex fields differ)
  std::vector<Mesh::Vertex> verts;
  verts.reserve(oldVerts.size());
  for (const auto& v : oldVerts) {
    Mesh::Vertex mv(v.Position,
                    glm::normalize(v.Position - center),
                    (v.TexCoord == glm::vec2(0)) ? glm::vec2(0) : v.TexCoord,
                    (v.Color == glm::vec4(0)) ? glm::vec4(1) : v.Color);
    verts.push_back(mv);
  }

  // 3) Create Mesh
  m_Mesh = std::make_unique<Mesh>(verts, indices);

  // 4) Create Material (shader + texture)
  m_Shader = std::make_shared<Shader>(
    Shaders::Camera::vertex,
    Shaders::Camera::fragment,
    std::vector<std::pair<std::string, unsigned>>{
      { "Camera", 0 } }); // :contentReference[oaicite:2]{index=2}

  m_Texture = std::make_shared<Texture>("res/textures/earth.jpg");
  m_Material = std::make_unique<Material>(m_Shader);

  m_Material->addTexture(1, m_Texture);
  // Optional: tweak pipeline state
  m_Material->state.depthTest = true;
  m_Material->state.depthWrite = true;
  m_Material->state.cull = false;

  // 5) Make a Renderable and set a nice spin
  Transform t{};
  t.m_Translation = { 0, 0, 0 };
  t.m_Scale = { 1, 1, 1 };
  t.m_Quat = glm::quat(glm::vec3(0, 0, 0));
  m_Sphere.mesh = m_Mesh.get();
  m_Sphere.material = m_Material.get();
  m_Sphere.transform = t;

  // Optional: rotate a bit to show it's 3D
  m_RotateSpeed = glm::radians(10.f); // deg/s
}

TestSphere::~TestSphere() {}

void
TestSphere::OnUpdate(float deltaTime)
{
}

void
TestSphere::OnRender(Renderer* renderer)
{

  renderer->Submit(m_Sphere);
}

void
TestSphere::OnImGuiRender(Camera& camera)
{

  static glm::vec3 tgt(0.f, 0.f, -800.f);
  static float dist = camera.distance();
  static float yaw = camera.yawDeg();
  static float pitch = camera.pitchDeg();

  ImGui::SliderFloat3("Camera target", &tgt.x, -3000.f, 3000.f);
  ImGui::SliderFloat("Distance", &dist, 100.003f, 800.f);

  ImGui::SliderFloat("Yaw (deg)", &yaw, -180.f, 180.f);
  ImGui::SliderFloat("Pitch (deg)", &pitch, -89.f, 89.f);

  ImGui::Separator();
  ImGui::SliderFloat3(
    "Translate", &m_Sphere.transform.m_Translation.x, -500.f, 500.f);
  ImGui::SliderFloat3("Scale", &m_Sphere.transform.m_Scale.x, 0.1f, 5.f);

  camera.setLookAtTarget(tgt);
  camera.setOrbit(dist, yaw, pitch);
}

}