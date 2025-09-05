#include <cmath>
#include <core/AppContext.h>
#include <core/entities/objects/Satellite.h>
#include <core/geometry/ObjectFactory.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace {
// Physical constants
constexpr double GM_EARTH = 3.986004418e14;  // m^3/s^2
constexpr double OMEGA_EARTH = 7.2921150e-5; // rad/s
constexpr double WGS84_A = 6378137.0;        // m (equatorial radius)

inline double
wrapTwoPi(double x)
{
  const double twoPi = glm::two_pi<double>();
  x = std::fmod(x, twoPi);
  return (x < 0.0) ? x + twoPi : x;
}

inline glm::dvec3
rotateZ(const glm::dvec3& v, double a)
{
  const double c = std::cos(a), s = std::sin(a);
  return { c * v.x - s * v.y, s * v.x + c * v.y, v.z };
}
}

Satellite::Satellite(AppContext& ctx,
                     const Orbit& orbit,
                     double epochTime,
                     Globe& globe)
  : m_Orbit(orbit)
  , m_Epoch(epochTime)
  , m_Position(0.0, 0.0, -800.0)
  , m_Renderable(ObjectFactory::CreateSatellite(glm::vec3(m_Position), ctx))
  , m_Globe(globe)
{
}

void
Satellite::Update(double timeSinceEpoch)
{
  m_Position = m_Orbit.positionECI((m_Epoch + timeSinceEpoch), m_Globe) +
               glm::dvec3(m_Globe.RenderTask()->transform.translation);
  m_Renderable.transform.translation = m_Position;
  m_Renderable.transform.scale = glm::vec3({ 1.0f, 1.0f, 1.0f });
}

glm::dvec3
Satellite::Orbit::positionECI(double t, Globe& globe) const
{
  // Unpack elements (angles in radians, a in meters, times in seconds)
  const double a = semiMajorAxis;
  const double e = eccentricity;
  const double inc = inclination;
  const double raan_ = raan;
  const double argp = argPerigee;

  // 1) Mean motion & mean anomaly
  const double n = std::sqrt(GM_EARTH / (a * a * a));
  double M = meanAnomalyAtEpoch + n * (t - epoch);
  M = wrapTwoPi(M);

  // 2) Solve Kepler's equation: M = E - e*sin(E)
  double E = M;
  for (int k = 0; k < 15; ++k) {
    const double f = E - e * std::sin(E) - M;
    const double fp = 1.0 - e * std::cos(E);
    const double dE = f / fp;
    E -= dE;
    if (std::abs(dE) < 1e-12)
      break;
  }

  // 3) True anomaly and radius
  const double cE = std::cos(E), sE = std::sin(E);
  const double nu = std::atan2(std::sqrt(1.0 - e * e) * sE, cE - e);
  const double r = a * (1.0 - e * cE);

  // 4) Position in orbital plane (PQW)
  const double cnu = std::cos(nu), snu = std::sin(nu);
  const glm::dvec3 rPQW(r * cnu, r * snu, 0.0);

  // 5) Rotate PQW -> ECI (GLM is column-major: pass columns)
  const double cO = std::cos(raan_), sO = std::sin(raan_);
  const double ci = std::cos(inc), si = std::sin(inc);
  const double cw = std::cos(argp), sw = std::sin(argp);

  const glm::dmat3 R(
    glm::dvec3(cO * cw - sO * sw * ci, sO * cw + cO * sw * ci, sw * si),
    glm::dvec3(-cO * sw - sO * cw * ci, -sO * sw + cO * cw * ci, cw * si),
    glm::dvec3(sO * si, -cO * si, ci));

  // ECI in meters
  const glm::dvec3 rECI_m = R * rPQW;

  // 6) Scale meters -> your world units using your globe's equatorial radius
  const double R_world =
    globe.Geometry().Radii().x + 100.0; // in your engine units
  const double metersToWorld = R_world / WGS84_A;
  return rECI_m * metersToWorld; // ECI in world units
}
