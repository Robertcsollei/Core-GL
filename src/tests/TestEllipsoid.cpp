#include "TestEllipsoid.h"
#include <glm/gtc/matrix_transform.hpp>

#include "TestUtils.h"
#include "data/cam.shader.h"
#include <imgui/imgui.h>

namespace test {

static Mesh*
BuildEllipsoidMesh(float radius,
                   int slices,
                   int stacks,
                   const glm::vec3& center,
                   Ellipsoid& ellipsoid)
{
  // 1) Build sphere using your existing helper (into your "old" Vertex type)
  std::vector<Vertex> oldVerts;
  std::vector<uint32_t> indices;
  test::AppendEllipsoid(
    oldVerts, indices, center, radius, slices, stacks, glm::vec4(1.f));

  // 2) Convert to Mesh::Vertex (adjust mapping if your Vertex fields differ)
  std::vector<Mesh::Vertex> verts;
  verts.reserve(oldVerts.size());
  for (const auto& v : oldVerts) {
    glm::dvec3 local = glm::dvec3(v.Position) - glm::dvec3(center);

    // geodetic normal expects origin-centered coords; then make it unit-length
    glm::dvec3 n = ellipsoid.GeodeticSurfaceNormal(local);
    glm::vec3 n32 = glm::normalize(glm::vec3(n));
    Mesh::Vertex mv(v.Position,
                    n32,
                    (v.TexCoord == glm::vec2(0)) ? glm::vec2(0) : v.TexCoord,
                    (v.Color == glm::vec4(0)) ? glm::vec4(1) : v.Color);
    verts.push_back(mv);
  }

  return new Mesh(verts, indices);
}

TestEllipsoid::TestEllipsoid()
  : m_Ellipsoid(glm::dvec3(100.0, 95.0, 100.0))
{
  m_Mesh.reset(BuildEllipsoidMesh(100.f, 64, 32, m_Center, m_Ellipsoid));

  m_Shader = std::make_shared<Shader>(
    Shaders::Camera::vertex,
    Shaders::Camera::fragment,
    std::vector<std::pair<std::string, unsigned>>{ { "Camera", 0 } });

  m_Texture = std::make_shared<Texture>("res/textures/earth.jpg");

  m_Material = std::make_unique<Material>(m_Shader);
  m_Material->addTexture(1, m_Texture);
  m_Material->state.depthTest = true;
  m_Material->state.depthWrite = true;
  m_Material->state.cull = false;

  Transform t{};
  t.m_Translation = { 0, 0, 0 };
  t.m_Scale = { 1, 1, 1 };
  t.m_Quat = glm::quat(glm::vec3(0, 0, 0));
  m_Sphere.mesh = m_Mesh.get();
  m_Sphere.material = m_Material.get();
  m_Sphere.transform = t;

  m_MarkerMesh.reset(
    BuildEllipsoidMesh(1.5f, 12, 8, glm::vec3(0), m_Ellipsoid));

  // Optional: rotate a bit to show it's 3D
  m_MarkerA = Renderable(m_MarkerMesh.get(), m_Material.get(), Transform{});
  m_MarkerB = Renderable(m_MarkerMesh.get(), m_Material.get(), Transform{});
  m_NormalTip = Renderable(m_MarkerMesh.get(), m_Material.get(), Transform{});
}

TestEllipsoid::~TestEllipsoid() {}

void
TestEllipsoid::OnUpdate(float deltaTime)
{
}

void
TestEllipsoid::OnRender(Renderer* renderer)
{
  // 1) Draw the globe
  renderer->Submit(m_Sphere);

  // 2) Compute geodetic points in ellipsoid’s local space (origin-centered)
  Geodetic3D gA(static_cast<double>(glm::radians(m_LatLonAdeg.x)),
                static_cast<double>(glm::radians(m_LatLonAdeg.y)),
                static_cast<double>(m_HeightA));
  Geodetic3D gB(static_cast<double>(glm::radians(m_LatLonBdeg.x)),
                static_cast<double>(glm::radians(m_LatLonBdeg.y)),
                static_cast<double>(m_HeightB));

  glm::dvec3 pA_local = m_Ellipsoid.ToVec3D(gA);
  glm::dvec3 pB_local = m_Ellipsoid.ToVec3D(gB);

  // World positions (your mesh is translated by m_Center)
  glm::vec3 pA = glm::vec3(pA_local) + m_Center;
  glm::vec3 pB = glm::vec3(pB_local) + m_Center;

  // Place the two point markers
  m_MarkerA.transform.m_Translation = pA;
  m_MarkerA.transform.m_Scale = glm::vec3(1.0f);
  m_MarkerA.transform.m_Quat = glm::quat(glm::vec3(0));

  m_MarkerB.transform.m_Translation = pB;
  m_MarkerB.transform.m_Scale = glm::vec3(1.0f);
  m_MarkerB.transform.m_Quat = glm::quat(glm::vec3(0));

  renderer->Submit(m_MarkerA);
  renderer->Submit(m_MarkerB);

  // 3) Visualize geodetic normal at A (little offset marker)
  if (m_ShowNormal) {
    // surface footpoint at A (height=0)
    glm::dvec3 sA_local =
      m_Ellipsoid.ToVec3D(Geodetic3D(gA.Latitude(), gA.Longitude(), 0.0));
    glm::dvec3 nHatA = m_Ellipsoid.GeodeticSurfaceNormal(
      Geodetic3D(gA.Latitude(), gA.Longitude(), 0.0));
    glm::vec3 tip = glm::vec3(sA_local + 8.0 * nHatA) + m_Center; // 8 units

    m_NormalTip.transform.m_Translation = tip;
    m_NormalTip.transform.m_Scale = glm::vec3(0.8f);
    m_NormalTip.transform.m_Quat = glm::quat(glm::vec3(0));
    renderer->Submit(m_NormalTip);
  }

  // 4) Curve between A and B (on the surface; ignore heights for the path)
  glm::dvec3 sA_local =
    m_Ellipsoid.ToVec3D(Geodetic3D(gA.Latitude(), gA.Longitude(), 0.0));
  glm::dvec3 sB_local =
    m_Ellipsoid.ToVec3D(Geodetic3D(gB.Latitude(), gB.Longitude(), 0.0));

  auto samples =
    m_Ellipsoid.ComputeCurve(sA_local, sB_local, std::max(2, m_CurveSamples));

  // Ensure we have enough tiny-sphere renderables
  if ((int)m_CurveMarkers.size() < (int)samples.size()) {
    size_t need = samples.size() - m_CurveMarkers.size();
    for (size_t i = 0; i < need; ++i) {
      m_CurveMarkers.emplace_back(
        m_MarkerMesh.get(), m_Material.get(), Transform{});
    }
  }

  // Place & draw them
  for (size_t i = 0; i < samples.size(); ++i) {
    glm::vec3 pos = glm::vec3(samples[i]) + m_Center;
    auto& R = m_CurveMarkers[i];
    R.transform.m_Translation = pos;
    R.transform.m_Scale = glm::vec3(0.7f);
    R.transform.m_Quat = glm::quat(glm::vec3(0));
    renderer->Submit(R);
  }
}

void
TestEllipsoid::OnImGuiRender(Camera&)
{
  ImGui::TextUnformatted("Geodetic debug (degrees, meters)");
  ImGui::Separator();
  ImGui::SliderFloat("A lat", &m_LatLonAdeg.x, -89.99, 89.99);
  ImGui::SliderFloat("A lon", &m_LatLonAdeg.y, -180.0, 180.0);
  ImGui::SliderFloat("A height", &m_HeightA, -5000.0, 50000.0);

  ImGui::Spacing();
  ImGui::SliderFloat("B lat", &m_LatLonBdeg.x, -89.99, 89.99);
  ImGui::SliderFloat("B lon", &m_LatLonBdeg.y, -180.0, 180.0);
  ImGui::SliderFloat("B height", &m_HeightB, -5000.0, 50000.0);

  ImGui::Spacing();
  ImGui::Checkbox("Show normal at A", &m_ShowNormal);
  ImGui::SliderInt("Curve samples", &m_CurveSamples, 8, 512);

  ImGui::TextDisabled(
    "Tip: set heights to 0 to see the surface normal and path cleanly.");
}
}