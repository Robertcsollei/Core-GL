#include <core/Scene.h>
#include <core/Window.h>
#include <core/entities/objects/Satellite.h>
#include <core/layers/GlobeLayer.h>
#include <core/layers/SatelliteLayer.h>
#include <iostream>
#include <memory>
#include <network/Fetch.h>
#include <projects/satelliteTracker/SatelliteTracker.h>
#include <renderer/Renderer.h>
#include <sstream>

#include <imgui/imgui.h>

static inline double
TleEpochToUnixSeconds(const std::string& l1)
{
  // TLE columns are 1-based: [19-20]=YY, [21-32]=DDD.DDD...
  // Zero-based substr: start at 18 (2 chars), then 20 (12 chars)
  int yy = std::stoi(l1.substr(18, 2));
  double day = std::stod(l1.substr(20, 12)); // DDD.dddddddd

  // Convert YY to full year (TLE epoch rule: 57–99 -> 1957–1999, 00–56 ->
  // 2000–2056)
  int year = (yy >= 57) ? (1900 + yy) : (2000 + yy);

  // day-of-year -> date
  // Build a tm at Jan 1st 00:00:00 UTC for that year
  std::tm tm = {};
  tm.tm_year = year - 1900;
  tm.tm_mon = 0;  // Jan
  tm.tm_mday = 1; // 1st
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;

  // We need timegm (UTC). If not available on Windows, implement a tiny timegm:
  auto timegm_portable = [](std::tm t) -> std::time_t {
#ifdef _WIN32
    // _mkgmtime is UTC version of mktime
    return _mkgmtime(&t);
#else
    return timegm(&t);
#endif
  };

  std::time_t jan1 = timegm_portable(tm);

  // Fractional day to seconds
  double secs = (day - 1.0) * 86400.0; // day-of-year is 1-based

  return static_cast<double>(jan1) + secs;
}

void
SatelliteTracker::Init()
{
  Application::Init();
  m_Scene->SatLayer()->Pause();
  FetchSatellitesData();
}

void
SatelliteTracker::Update(double deltaTime)
{
  OrbitWithName orbitWithName;
  auto* satelliteLayer = m_Scene->SatLayer();
  auto* globeLayer = m_Scene->GlobeLayer();
  while (m_OrbitQueue.try_pop(orbitWithName)) {
    auto sat = std::make_unique<Satellite>(
      m_Ctx, orbitWithName.name, orbitWithName.orbit, 0.0, globeLayer->globe());
    satelliteLayer->Add(std::move(sat));
    if (!m_FetchingData && m_OrbitQueue.isEmpty()) {
      satelliteLayer->Resume();
    }
  }
}

void
SatelliteTracker::Render()
{
}

void
SatelliteTracker::RenderUI()
{
  ImGui::DragScalar("Satellite Speed Multiplier",
                    ImGuiDataType_::ImGuiDataType_Double,
                    &m_Scene->state().satelliteSpeedMultiplier);
}

void
SatelliteTracker::FetchSatellitesData()
{
  std::cout << "Loading satellites..."
            << "\n";
  static const std::string allSatellites =
    "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";
  static const std::string spaceStations =
    "https://celestrak.org/NORAD/elements/gp.php?GROUP=stations&FORMAT=tle";
  constexpr double mu = 3.986004418e14; // [m^3/s^2]
  m_FetchingData = true;
  Fetch::Get(
    allSatellites,
    [this](const std::string& body) { // onSuccess
      std::stringstream ss(body);
      std::string line;
      int subLine = 0;
      std::string name, l1, l2;

      while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') {
          line.pop_back();
        }

        if (subLine == 0) {
          name = line;
        } else if (subLine == 1) {
          l1 = line;
        } else if (subLine == 2) {
          l2 = line;

          Satellite::Orbit orbit{};
          OrbitWithName orbitWithName{};

          // Parse line 2 (fixed-column format)
          double i = std::stod(l2.substr(8, 8));     // inclination [deg]
          double raan = std::stod(l2.substr(17, 8)); // RAAN [deg]
          double ecc = std::stod(l2.substr(26, 7)); // eccentricity (no decimal)
          double w = std::stod(l2.substr(34, 8));   // argument of perigee [deg]
          double M0 = std::stod(l2.substr(43, 8));  // mean anomaly [deg]
          double n = std::stod(l2.substr(52, 11));  // mean motion [rev/day]

          orbit.inclination = glm::radians(i);
          orbit.raan = glm::radians(raan);
          orbit.eccentricity = ecc * 1e-7;
          orbit.argPerigee = glm::radians(w);
          orbit.meanAnomalyAtEpoch = glm::radians(M0);

          double n_rad = n * 2.0 * M_PI / 86400.0; // convert rev/day -> rad/s
          orbit.semiMajorAxis = cbrt(mu / (n_rad * n_rad));

          orbit.epoch = TleEpochToUnixSeconds(l1);

          orbitWithName.orbit = std::move(orbit);
          orbitWithName.name = name;
          m_OrbitQueue.push(std::move(orbitWithName));
        }

        subLine = (subLine + 1) % 3;
      }

      m_FetchingData = false;
      std::cout << "End of file!"
                << "\n";
    },
    [](const std::string& error) { // onError
      std::cerr << "[ERROR] " << error << std::endl;
    });
}
