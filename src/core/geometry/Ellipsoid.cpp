#include <core/geometry/Ellipsoid.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Ellipsoid::Ellipsoid(glm::dvec3 radii)
  : m_Radii(radii)
  , m_RadiiSquared(radii * radii)
  , m_RadiiToTheFourth(m_RadiiSquared * m_RadiiSquared)
  , m_OneOverRadiiSquared(1.0 / m_RadiiSquared)
{
}

glm::dvec3
Ellipsoid::GeodeticSurfaceNormal(const glm::dvec3& p)
{

  return glm::normalize(p * m_OneOverRadiiSquared);
}

glm::dvec3
Ellipsoid::GeodeticSurfaceNormal(const Geodetic3D& geodetic)
{
  double cosLat = glm::cos(geodetic.Latitude());

  return glm::dvec3{ cosLat * glm::cos(geodetic.Longitude()),
                     cosLat * glm::sin(geodetic.Longitude()),
                     glm::sin(geodetic.Latitude()) };
}

glm::dvec3
Ellipsoid::ToVec3D(const Geodetic3D& geodetic)
{
  glm::dvec3 n = GeodeticSurfaceNormal(geodetic);
  glm::dvec3 k = m_RadiiSquared * n;
  double gamma = glm::sqrt(k.x * n.x + k.y * n.y + k.z * n.z);
  glm::dvec3 surface = k / gamma;
  return surface + (geodetic.Height() * n);
}

Geodetic2D
Ellipsoid::ToGeodetic2D(const glm::dvec3& p)
{
  glm::dvec3 n = GeodeticSurfaceNormal(p);
  return Geodetic2D(glm::atan2(n.y, n.x), glm::asin(n.z / n.length()));
}

Geodetic3D
Ellipsoid::ToGeodetic3D(const glm::dvec3& p)
{
  glm::dvec3 s = ScaleToGeodeticSurface(p);
  glm::dvec3 n = s * m_OneOverRadiiSquared;
  glm::dvec3 nHat = glm::normalize(n);
  double height = glm::dot(p - s, nHat);
  return Geodetic3D(ToGeodetic2D(s), height);
}

glm::dvec3
Ellipsoid::ScaleToGeocentricSurface(const glm::dvec3& p)
{
  glm::dvec3 p2 = p * p;
  double beta = 1.0 / glm::sqrt(glm::dot(p2, m_OneOverRadiiSquared));
  return beta * p;
}

glm::dvec3
Ellipsoid::ScaleToGeodeticSurface(const glm::dvec3& p)
{
  glm::dvec3 p2 = p * p;
  double beta = 1.0 / glm::sqrt(glm::dot(p2, m_OneOverRadiiSquared));

  double n = glm::dvec3(p * m_OneOverRadiiSquared * beta).length();
  double alpha = (1.0 - beta) * (p.length() / n);
  glm::dvec3 d(0.0);
  double s = 0.0;
  double dSdA = 1.0;

  do {
    alpha -= (s / dSdA);
    d = 1.0 + (alpha * m_OneOverRadiiSquared);
    glm::dvec3 d2 = d * d;
    glm::dvec3 d3 = d * d2;

    glm::dvec3 sT = p2 / (m_RadiiSquared * d2);
    glm::dvec3 dSdAT = p2 / (m_RadiiToTheFourth * d3);

    s = sT.x + sT.y + sT.z - 1.0;
    dSdA = -2.0 * (dSdAT.x + dSdAT.y + dSdAT.z);

  } while (glm::abs(s) > 1e-10);

  return p / d;
}

std::vector<glm::dvec3>
Ellipsoid::ComputeCurve(const glm::dvec3& p, const glm::dvec3& q, int samples)
{
  std::vector<glm::dvec3> positions;
  samples = std::max(2, samples);
  positions.reserve(samples + 1);

  glm::dvec3 u = glm::normalize(p);
  glm::dvec3 v = glm::normalize(q);

  double dotUV = glm::clamp(glm::dot(u, v), -1.0, 1.0);
  double theta = std::acos(dotUV);

  if (theta < 1e-12) {
    positions.push_back(ScaleToGeocentricSurface(p));
    positions.push_back(ScaleToGeocentricSurface(q));
    return positions;
  }
  glm::dvec3 axis = glm::cross(u, v);
  if (glm::length2(axis) < 1e-24) {
    axis = glm::cross(u, glm::dvec3(1, 0, 0));
    if (glm::length2(axis) < 1e-24) {
      axis = glm::cross(u, glm::dvec3(0, 1, 0));
    }
  }

  glm::dvec3 nHat = glm::normalize(axis);

  double step = theta / static_cast<double>(samples);
  positions.push_back(ScaleToGeocentricSurface(u));

  for (int i = 1; i < samples; ++i) {
    double ang = step * static_cast<double>(i);
    glm::dvec3 dir = glm::rotate(u, ang, nHat);
    positions.push_back(ScaleToGeocentricSurface(dir));
  }
  positions.push_back(v);
  return positions;
}

const Ellipsoid Ellipsoid::wgs84{
  glm::dvec3{ 6378137.0, 6378137.0, 6356752.314245 }
};
const Ellipsoid Ellipsoid::unitSphere{ glm::dvec3{ 1.0, 1.0, 1.0 } };