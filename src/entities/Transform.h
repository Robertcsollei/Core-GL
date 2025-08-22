#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
  glm::quat m_Quat;
  glm::vec3 m_Euler, m_Pivot, m_Translation, m_Scale;

  glm::mat4 world() const
  {
    glm::mat4 M(1.0f);
    M = glm::translate(M, m_Translation);
    M = glm::translate(M, m_Pivot);
    M *= glm::mat4_cast(m_Quat);
    M = glm::scale(M, m_Scale);
    M = glm::translate(M, -m_Pivot);
    return M;
  }

  Transform()
    : m_Quat(1.0f, 0.0f, 0.0f, 0.0f)
    , m_Euler(0.0f)
    , m_Pivot(0.0f)
    , m_Translation(0.0f)
    , m_Scale(1.0f)
  {
  }
  Transform(const glm::quat& quat,
            const glm::vec3& euler,
            const glm::vec3& pivot,
            const glm::vec3& translation,
            const glm::vec3& scale)
    : m_Quat(quat)
    , m_Euler(euler)
    , m_Pivot(pivot)
    , m_Translation(translation)
    , m_Scale(scale)
  {
  }
};