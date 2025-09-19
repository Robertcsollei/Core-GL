#pragma once
#include <terrakit/core/Renderable.h>
#include <terrakit/core/SceneState.h>
#include <terrakit/core/entities/Globe.h>

class AppContext;

namespace terrakit::core {
class Satellite
{
public:
  struct State
  {
    enum Type
    {
      Default = 0,
      Active,
      Hovered,

    };
    bool hovered = false;
    bool active = false;

    Type type() const
    {
      if (active)
        return Active;
      if (hovered)
        return Hovered;
      return Default;
    }
  };

  struct Orbit
  {
    double semiMajorAxis = 6778137.0; // meters;
    double eccentricity;
    double inclination;
    double raan;
    double argPerigee;
    double meanAnomalyAtEpoch;
    double epoch;

    glm::dvec3 positionECI(double t, Globe& globe, double multiplier) const;
  };

  struct PrecomputedOrbit
  {
    double a;  // semi-major axis
    double e;  // eccentricity
    double M0; // mean anomaly at epoch
    double n;  // mean motion (rad/s)
    double epoch;
    glm::dmat3 R_pf2eci;
  };

public:
  Satellite(AppContext& ctx,
            const std::string uuid,
            const Orbit& orbit,
            double epochTime,
            Globe& globe);

  ~Satellite() = default;

  void Update(double timeSinceEpoch, const SceneState& sceneState);
  Renderable* RenderTask() { return &m_Renderable; }

  void PrecomputeOrbit();
  glm::dvec3 GetPrecomputedPos(double t, double multiplier);

public:
  const glm::dvec3& position() const { return m_Position; }
  const glm::vec3& renderPos() const { return m_RenderPos; }

  double* epoch() { return &m_Epoch; }
  const std::string& uuid() const { return m_Uuid; }

  const PrecomputedOrbit& precomputed() { return m_PrecomputedOrbit; };

  State state;

private:
  const std::string m_Uuid;

  glm::dvec3 m_Position;
  glm::vec4 m_Color;

  glm::vec3 m_RenderPos;

  Orbit m_Orbit;
  PrecomputedOrbit m_PrecomputedOrbit;
  double m_Epoch;
  Renderable m_Renderable;
  Globe& m_Globe;
};
} // namespace terrakit::core