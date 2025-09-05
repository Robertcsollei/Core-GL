#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
  glm::quat quat;
  glm::vec3 euler, pivot, translation, scale;

  glm::mat4 world() const
  {
    glm::mat4 M(1.0f);
    M = glm::translate(M, translation);
    M = glm::translate(M, pivot);
    M *= glm::mat4_cast(quat);
    M = glm::scale(M, scale);
    M = glm::translate(M, -pivot);
    return M;
  }

  Transform()
    : quat(1.0f, 0.0f, 0.0f, 0.0f)
    , euler(0.0f)
    , pivot(0.0f)
    , translation(0.0f)
    , scale(1.0f)
  {
  }
  Transform(const glm::quat& quat,
            const glm::vec3& euler,
            const glm::vec3& pivot,
            const glm::vec3& translation,
            const glm::vec3& scale)
    : quat(quat)
    , euler(euler)
    , pivot(pivot)
    , translation(translation)
    , scale(scale)
  {
  }
};