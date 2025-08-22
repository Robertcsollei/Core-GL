#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
  struct alignas(16) UBO
  {
    glm::mat4 view; // 64 bytes
    glm::mat4 proj; // 64 bytes
    glm::vec4 eye;  // 16 bytes (xyz + padding)
  };

public:
  Camera(float fovDeg, float width, float height, float nearZ, float farZ);
  ~Camera() = default;

public:
  void setLookAt(const glm::vec3& eye,
                 const glm::vec3& target,
                 const glm::vec3& up);
  void setLookAtTarget(const glm::vec3& target);
  void setOrbit(float distance, float yawDeg, float pitchDeg); // absolute
  void addOrbitDelta(float dYawDeg, float dPitchDeg);          // relative

  void setFov(float fovDeg);
  void setAspect(float width, float height);
  void onResize(float width, float height);

  const glm::mat4& view() const { return m_View; }
  const glm::mat4& proj() const { return m_Proj; }
  const glm::vec3& position() const { return m_Eye; }

  float distance() const { return m_Dist; }
  float yawDeg() const { return m_YawDeg; }
  float pitchDeg() const { return m_PitchDeg; }
  const glm::vec3& target() const { return m_Target; }

private:
  glm::mat4 m_View{ 1 }, m_Proj{ 1 };
  glm::vec3 m_Eye{ 0 }, m_Target{ 0, 0, -1 }, m_Up{ 0, 1, 0 };
  float m_Fov, m_Aspect, m_Near, m_Far;

  float m_Dist = 300.f;
  float m_YawDeg = 0.f; // 0° looks along +Z toward the target
  float m_PitchDeg = 0.f;

private:
  void rebuildProj();
  void updateViewFromOrbit();
};
