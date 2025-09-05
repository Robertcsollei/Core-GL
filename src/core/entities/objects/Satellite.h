#pragma once
#include <core/entities/Renderable.h>
#include <core/entities/objects/Globe.h>

class AppContext;

class Satellite
{
public:
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

public:
  Satellite(AppContext& ctx,
            const Orbit& orbit,
            double epochTime,
            Globe& globe);

  ~Satellite() = default;

  void Update(double timeSinceEpoch);
  Renderable* RenderTask() { return &m_Renderable; }

public:
  const glm::dvec3& position() const { return m_Position; }
  double* epoch() { return &m_Epoch; }

private:
  glm::dvec3 m_Position;
  Orbit m_Orbit;
  double m_Epoch;
  Renderable m_Renderable;
  Globe& m_Globe;
};