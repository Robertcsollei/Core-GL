#include "Camera.h"
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovDeg, float width, float height, float nearZ, float farZ)

  : m_Mode(Lens::Perspective)
  , m_Fov(fovDeg)
  , m_Aspect(width / height)
  , m_Near(nearZ)
  , m_Far(farZ)
{
  setLookAt({ 0, 0, 3 }, { 0, 0, 0 }, { 0, 1, 0 });
  rebuildProj();
}

void
Camera::setLookAt(const glm::vec3& eye,
                  const glm::vec3& target,
                  const glm::vec3& up)
{
  m_Eye = eye;
  m_Target = target;
  m_Up = up;

  m_View = glm::lookAt(m_Eye, m_Target, m_Up);
}

void
Camera::setLookAtTarget(const glm::vec3& target)
{
  if (target == m_Target)
    return;
  m_Target = target;
  updateViewFromOrbit();
}

void
Camera::setOrbit(float distance, float yawDeg, float pitchDeg)
{
  if (distance == m_Dist && yawDeg == m_YawDeg && pitchDeg == m_PitchDeg)
    return;

  m_Dist = distance;
  m_YawDeg = yawDeg;
  m_PitchDeg = pitchDeg;
  updateViewFromOrbit();
}

void
Camera::addOrbitDelta(float dYawDeg, float dPitchDeg)
{
  m_YawDeg += dYawDeg;
  m_PitchDeg += dPitchDeg;
  updateViewFromOrbit();
}

void
Camera::setOrthoHeight(float h)
{
  m_OrthoHeight = h;
  if (m_Mode == Lens::Ortho)
    rebuildProj();
}

void
Camera::setFov(float fovDeg)
{
  m_Fov = fovDeg;
  rebuildProj();
}

void
Camera::onResize(float width, float height)
{
  m_Aspect = width / height;
  rebuildProj();
}

void
Camera::setPerspective(float fovDeg, float aspect, float nearZ, float farZ)
{
  m_Mode = Lens::Perspective;
  m_Fov = fovDeg;
  m_Aspect = aspect;
  m_Near = nearZ;
  m_Far = farZ;
  rebuildProj();
}

void
Camera::setOrthographic(float height, float aspect, float nearZ, float farZ)
{
  m_Mode = Lens::Ortho;
  m_OrthoHeight = height;
  m_Aspect = aspect;
  m_Near = nearZ;
  m_Far = farZ;
  rebuildProj();
}

void
Camera::setMode(Lens mode)
{
  if (mode == m_Mode)
    return;
  if (mode == Lens::Ortho) {
    m_OrthoHeight = 2.0f * m_Dist * std::tan(glm::radians(m_Fov * 0.5f));
  } else {
    m_Dist = std::max(
      0.01f, m_OrthoHeight / (2.0f * std::tan(glm::radians(m_Fov * 0.5f))));
  }
  m_Mode = mode;
  rebuildProj();
}

void
Camera::setAspect(float width, float height)
{
  m_Aspect = width / height;
  rebuildProj();
}

void
Camera::rebuildProj()
{
  if (m_Mode == Lens::Perspective) {
    m_Proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
  } else {
    float h = m_OrthoHeight;
    float w = h * m_Aspect;
    m_Proj =
      glm::ortho(-0.5f * w, 0.5f * w, -0.5f * h, 0.5f * h, m_Near, m_Far);
  }
}

void
Camera::updateViewFromOrbit()
{
  // Clamp pitch to avoid gimbal flip, clamp distance
  m_PitchDeg = std::clamp(m_PitchDeg, -89.0f, 89.0f);
  m_Dist = std::max(0.01f, m_Dist);

  const float yaw = glm::radians(m_YawDeg);
  const float pitch = glm::radians(m_PitchDeg);
  const float cp = std::cos(pitch), sp = std::sin(pitch);
  const float sy = std::sin(yaw), cy = std::cos(yaw);

  // Spherical to Cartesian around target
  glm::vec3 offset(cp * sy, sp, cp * cy); // yaw=0 => +Z
  m_Eye = m_Target + m_Dist * offset;

  m_View = glm::lookAt(m_Eye, m_Target, m_Up);
}
