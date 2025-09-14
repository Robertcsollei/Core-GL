#pragma once
#include <core/SceneState.h>
#include <core/entities/Renderable.h>
#include <core/entities/objects/Globe.h>

class AppContext;

class Satellite
{
public:
  struct ObjectState
  {
    bool hovered = false;
    bool active = false;
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

    glm::dvec3 positionECI(double t, Globe& globe) const;
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
  glm::dvec3 GetPrecomputedPos(double t);

public:
  const glm::dvec3& position() const { return m_Position; }
  const glm::vec3& renderPos() const { return m_RenderPos; }
  uint32_t color() const;

  double* epoch() { return &m_Epoch; }
  const std::string& uuid() const { return m_Uuid; }

  const PrecomputedOrbit& precomputed() { return m_PrecomputedOrbit; };

  static constexpr uint32_t DefaultColor = 0xFFFFFFFF;
  static constexpr uint32_t HoverColor = 0xFF0000FF;
  static constexpr uint32_t SelectColor = 0x0000FF00;

  ObjectState state;

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
