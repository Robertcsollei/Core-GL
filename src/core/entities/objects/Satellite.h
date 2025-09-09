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
            const std::string uuid,
            const Orbit& orbit,
            double epochTime,
            Globe& globe);

  ~Satellite() = default;

  void Update(double timeSinceEpoch);
  Renderable* RenderTask() { return &m_Renderable; }
  void SetColor(const glm::vec4& color) { m_Color = color; }

public:
  const glm::dvec3& position() const { return m_Position; }
  const glm::vec3& renderPos() const { return m_RenderPos; }
  const glm::vec4& color() const { return m_Color; }

  double* epoch() { return &m_Epoch; }
  const std::string& uuid() const { return m_Uuid; }

  static constexpr glm::vec4 DefaultColor = { 1.f, 0.f, 0.f, 1.f };
  static constexpr glm::vec4 HoverColor = { 0.f, 1.f, 0.f, 1.f };

private:
  const std::string m_Uuid;

  glm::dvec3 m_Position;
  glm::vec4 m_Color;

  glm::vec3 m_RenderPos;

  Orbit m_Orbit;
  double m_Epoch;
  Renderable m_Renderable;
  Globe& m_Globe;
};