#pragma once
#include "core/Geodetic2D.h"
#include "core/Geodetic3D.h"
#include <glm/glm.hpp>
#include <vector>

class Ellipsoid
{

public:
  Ellipsoid(glm::dvec3 radii);
  ~Ellipsoid() = default;

  glm::dvec3 Radii() { return m_Radii; }
  glm::dvec3 GeodeticSurfaceNormal(const glm::dvec3& p);
  glm::dvec3 GeodeticSurfaceNormal(const Geodetic3D& geodetic);
  glm::dvec3 ToVec3D(const Geodetic3D& geodetic);
  glm::dvec3 ScaleToGeocentricSurface(const glm::dvec3& p);
  glm::dvec3 ScaleToGeodeticSurface(const glm::dvec3& p);

  std::vector<glm::dvec3> ComputeCurve(const glm::dvec3& p,
                                       const glm::dvec3& q,
                                       int samples);

  Geodetic2D ToGeodetic2D(const glm::dvec3& p);
  Geodetic3D ToGeodetic3D(const glm::dvec3& p);

  static const Ellipsoid wgs84;
  static const Ellipsoid unitSphere;

private:
  glm::dvec3 m_Radii;
  glm::dvec3 m_RadiiSquared;
  glm::dvec3 m_RadiiToTheFourth;
  glm::dvec3 m_OneOverRadiiSquared;
};