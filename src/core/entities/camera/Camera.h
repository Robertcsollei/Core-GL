#pragma once
#include <glm/glm.hpp>
#include <memory>
class Camera
{
public:
  struct alignas(16) UBO
  {
    glm::mat4 view; // 64 bytes
    glm::mat4 proj; // 64 bytes
    glm::vec4 eye;  // 16 bytes (xyz + padding)
  };

  enum class Lens
  {
    Perspective,
    Ortho
  };

public:
  Camera(float fovDeg, float width, float height, float nearZ, float farZ);
  ~Camera() = default;

  static std::unique_ptr<Camera> CreateDefaultCamera(float fbW, float fbH);

public:
  void SetLookAt(const glm::vec3& eye,
                 const glm::vec3& target,
                 const glm::vec3& up);
  void SetLookAtTarget(const glm::vec3& target);
  void SetOrbit(float distance, float yawDeg, float pitchDeg); // absolute
  void AddOrbitDelta(float dYawDeg, float dPitchDeg);          // relative
  void SetOrthoHeight(float h);
  void SetFov(float fovDeg);
  void SetAspect(float width, float height);
  void OnResize(float width, float height);

  void SetPerspective(float fovDeg, float aspect, float nearZ, float farZ);
  void SetOrthographic(float height, float aspect, float nearZ, float farZ);
  void SetMode(Lens mode);

  glm::dvec3 CastRay(double ndcX, double ndcY);
  glm::ivec2 WorldToScreen(const glm::dvec3& pos, float screenW, float screenH);

  const glm::mat4& view() const { return m_View; }
  const glm::mat4& proj() const { return m_Proj; }
  const glm::vec3& position() const { return m_Eye; }
  const glm::vec3& target() const { return m_Target; }

  float distance() const { return m_Dist; }
  float yawDeg() const { return m_YawDeg; }
  float pitchDeg() const { return m_PitchDeg; }
  float fovDeg() const { return m_Fov; }
  float orthoHeight() const { return m_OrthoHeight; }
  float near() const { return m_Near; }
  float far() const { return m_Far; }

  Lens mode() const { return m_Mode; }

private:
  glm::mat4 m_View{ 1 }, m_Proj{ 1 };
  glm::vec3 m_Eye{ 0 }, m_Target{ 0, 0, -1 }, m_Up{ 0, 1, 0 };
  float m_Fov = 45.f, m_Aspect = 16.f / 9.f, m_Near, m_Far;

  // Orbit
  float m_Dist = 300.f;
  float m_YawDeg = 0.f; // 0° looks along +Z toward the target
  float m_PitchDeg = 0.f;

  // Lens
  Lens m_Mode;
  float m_OrthoHeight = 300.f;

private:
  void RebuildProj();
  void UpdateViewFromOrbit();
};
