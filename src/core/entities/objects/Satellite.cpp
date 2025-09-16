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

static glm::dmat3
rotZ(double a)
{
  double s = std::sin(a), c = std::cos(a);
  return glm::dmat3(c, -s, 0, s, c, 0, 0, 0, 1);
}
static glm::dmat3
rotX(double a)
{
  double s = std::sin(a), c = std::cos(a);
  return glm::dmat3(1, 0, 0, 0, c, -s, 0, s, c);
}

// Optional: ECI -> engine basis if you want Y-up, Z-forward
inline glm::dvec3
EciToEngine(const glm::dvec3& eci)
{
  // Example: keep X, swap Y/Z, flip Z to match your camera
  // Adjust this to your world; the key is to be consistent.
  return { eci.x, eci.z, -eci.y };
}
}

Satellite::Satellite(AppContext& ctx,
                     const std::string uuid,
                     const Orbit& orbit,
                     double epochTime,
                     Globe& globe)
  : m_Uuid(uuid)
  , m_Orbit(orbit)
  , m_Epoch(epochTime)
  , m_Position(0.0, 0.0, -800.0)
  , m_Color(1.f)
  , m_RenderPos(0.f)
  , m_PrecomputedOrbit()
  , m_Renderable(ObjectFactory::CreateSatellite(glm::vec3(m_Position), ctx))
  , m_Globe(globe)
{
  PrecomputeOrbit();
}

void
Satellite::Update(double timeSinceEpoch, const SceneState& sceneState)
{
  if (!state.active) {
    m_Position = GetPrecomputedPos(
      m_Epoch + timeSinceEpoch * sceneState.satelliteSpeedMultiplier);
  } else {
    m_Position = m_Orbit.positionECI((m_Epoch + timeSinceEpoch) *
                                       sceneState.satelliteSpeedMultiplier,
                                     m_Globe);
  }
  m_RenderPos = glm::vec3(m_Position);

  m_Renderable.transform.translation = m_Position;
  m_Renderable.transform.scale = glm::vec3({ 1.0f, 1.0f, 1.0f });
}

glm::dvec3
Satellite::GetPrecomputedPos(double t)
{
  // Use (t - epoch) to match the slow path
  double M = m_PrecomputedOrbit.M0 +
             m_PrecomputedOrbit.n * (t - m_PrecomputedOrbit.epoch);
  M = wrapTwoPi(M);

  // Kepler solver
  double E = M;
  for (int i = 0; i < 2; ++i) {
    double f = E - m_PrecomputedOrbit.e * std::sin(E) - M;
    double fp = 1.0 - m_PrecomputedOrbit.e * std::cos(E);
    E -= f / fp;
  }

  double cE = std::cos(E), sE = std::sin(E);
  double nu = std::atan2(
    std::sqrt(1.0 - m_PrecomputedOrbit.e * m_PrecomputedOrbit.e) * sE,
    cE - m_PrecomputedOrbit.e);
  double r = m_PrecomputedOrbit.a * (1.0 - m_PrecomputedOrbit.e * cE);

  glm::dvec3 r_pf(r * std::cos(nu), r * std::sin(nu), 0.0);

  glm::dvec3 r_eci = m_PrecomputedOrbit.R_pf2eci * r_pf;
  glm::dvec3 rEngine_m = EciToEngine(r_eci);
  double R_world = m_Globe.Geometry().Radii().x;
  double metersToWorld = R_world / WGS84_A;
  return rEngine_m * metersToWorld +
         glm::dvec3(m_Globe.RenderTask()->transform.translation);
}

void
Satellite::PrecomputeOrbit()
{
  m_PrecomputedOrbit.a = m_Orbit.semiMajorAxis;
  m_PrecomputedOrbit.e = m_Orbit.eccentricity;
  m_PrecomputedOrbit.M0 = m_Orbit.meanAnomalyAtEpoch;
  m_PrecomputedOrbit.n =
    std::sqrt(GM_EARTH / std::pow(m_PrecomputedOrbit.a, 3));
  m_PrecomputedOrbit.epoch = m_Orbit.epoch;

  double cO = std::cos(m_Orbit.raan), sO = std::sin(m_Orbit.raan);
  double ci = std::cos(m_Orbit.inclination), si = std::sin(m_Orbit.inclination);
  double cw = std::cos(m_Orbit.argPerigee), sw = std::sin(m_Orbit.argPerigee);

  m_PrecomputedOrbit.R_pf2eci = glm::dmat3(
    glm::dvec3(cO * cw - sO * sw * ci, sO * cw + cO * sw * ci, sw * si),
    glm::dvec3(-cO * sw - sO * cw * ci, -sO * sw + cO * cw * ci, cw * si),
    glm::dvec3(sO * si, -cO * si, ci));
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
  glm::dvec3 rEngine_m = EciToEngine(rECI_m);

  // 6) Scale meters -> your world units using your globe's equatorial radius
  const double R_world = globe.Geometry().Radii().x; // in your engine units
  const double metersToWorld = R_world / WGS84_A;
  return rEngine_m * metersToWorld +
         glm::dvec3(
           globe.RenderTask()->transform.translation); // ECI in world units
}
