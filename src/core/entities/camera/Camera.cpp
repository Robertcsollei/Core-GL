#include <algorithm>
#include <cmath>
#include <core/entities/camera/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovDeg, float width, float height, float nearZ, float farZ)

  : m_Mode(Lens::Perspective)
  , m_Fov(fovDeg)
  , m_Aspect(width / height)
  , m_Near(nearZ)
  , m_Far(farZ)
{
  SetLookAt({ 0, 0, 3 }, { 0, 0, 0 }, { 0, 1, 0 });
  RebuildProj();
}

std::unique_ptr<Camera>
Camera::CreateDefaultCamera(float fbW, float fbH)
{
  auto camera = std::make_unique<Camera>(45.f, fbW, fbH, 0.1f, 10000.f);
  camera->SetLookAt({ 0, 0, 300 }, { 0, 0, -800 }, { 0, 1, 0 });
  return camera;
}

void
Camera::SetLookAt(const glm::vec3& eye,
                  const glm::vec3& target,
                  const glm::vec3& up)
{
  m_Eye = eye;
  m_Target = target;
  m_Up = up;

  m_View = glm::lookAt(m_Eye, m_Target, m_Up);
}

void
Camera::SetLookAtTarget(const glm::vec3& target)
{
  if (target == m_Target)
    return;
  m_Target = target;
  UpdateViewFromOrbit();
}

void
Camera::SetOrbit(float distance, float yawDeg, float pitchDeg)
{
  if (distance == m_Dist && yawDeg == m_YawDeg && pitchDeg == m_PitchDeg)
    return;

  m_Dist = distance;
  m_YawDeg = yawDeg;
  m_PitchDeg = pitchDeg;
  UpdateViewFromOrbit();
}

void
Camera::AddOrbitDelta(float dYawDeg, float dPitchDeg)
{
  m_YawDeg += dYawDeg;
  m_PitchDeg += dPitchDeg;
  UpdateViewFromOrbit();
}

void
Camera::SetOrthoHeight(float h)
{
  m_OrthoHeight = h;
  if (m_Mode == Lens::Ortho)
    RebuildProj();
}

void
Camera::SetFov(float fovDeg)
{
  m_Fov = fovDeg;
  RebuildProj();
}

void
Camera::OnResize(float width, float height)
{
  m_Aspect = width / height;
  RebuildProj();
}

void
Camera::SetPerspective(float fovDeg, float aspect, float nearZ, float farZ)
{
  m_Mode = Lens::Perspective;
  m_Fov = fovDeg;
  m_Aspect = aspect;
  m_Near = nearZ;
  m_Far = farZ;
  RebuildProj();
}

void
Camera::SetOrthographic(float height, float aspect, float nearZ, float farZ)
{
  m_Mode = Lens::Ortho;
  m_OrthoHeight = height;
  m_Aspect = aspect;
  m_Near = nearZ;
  m_Far = farZ;
  RebuildProj();
}

void
Camera::SetMode(Lens mode)
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
  RebuildProj();
}

glm::dvec3
Camera::CastRay(double ndcX, double ndcY)
{

  glm::dvec4 clipRay = glm::dvec4(ndcX, ndcY, -1.0, 1.0);

  glm::dvec4 eyeRay = glm::inverse(m_Proj) * clipRay;
  eyeRay = glm::dvec4(eyeRay.x, eyeRay.y, -1.0, 0.0);

  glm::dvec3 worldRay =
    glm::normalize(glm::dvec3(glm::inverse(m_View) * eyeRay));

  return worldRay;
}

glm::ivec2
Camera::WorldToScreen(const glm::dvec3& pos, float screenW, float screenH)
{
  glm::dvec4 clip = m_Proj * m_View * glm::dvec4(pos, 1.0);
  glm::dvec3 ndc = glm::dvec3(clip) / clip.w;

  int screenX = int((ndc.x * 0.5 + 0.5) * screenW);
  int screenY = int((1.0 - (ndc.y * 0.5 + 0.5)) * screenH);

  return glm::ivec2(screenX, screenY);
}

void
Camera::SetAspect(float width, float height)
{
  m_Aspect = width / height;
  RebuildProj();
}

void
Camera::RebuildProj()
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
Camera::UpdateViewFromOrbit()
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
