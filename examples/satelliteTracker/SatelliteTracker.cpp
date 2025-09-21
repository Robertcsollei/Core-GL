
#include "SatelliteTracker.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <terrakit/core/Logger.h>
#include <terrakit/core/Scene.h>
#include <terrakit/core/Window.h>
#include <terrakit/core/entities/Satellite.h>
#include <terrakit/core/layers/GlobeLayer.h>
#include <terrakit/core/layers/SatelliteLayer.h>
#include <terrakit/network/Fetch.h>
#include <terrakit/renderer/Renderer.h>

#include <imgui/imgui.h>

#define TERRAKIT_MOCK ;

using namespace terrakit::core;
using namespace terrakit::network;

ImVec4
ToImVec4(uint32_t c)
{

  return ImVec4((c & 0xFF) / 255.0f,         // R
                ((c >> 8) & 0xFF) / 255.0f,  // G
                ((c >> 16) & 0xFF) / 255.0f, // B
                1.0);
}

uint32_t
FromImVec4(ImVec4 v)
{
  uint8_t R = (uint8_t)(v.x * 255);
  uint8_t G = (uint8_t)(v.y * 255);
  uint8_t B = (uint8_t)(v.z * 255);
  uint8_t A = 255; // Always fully opaque

  return (A << 24) | (B << 16) | (G << 8) | R;
}

/**
 * @brief Converts Two-Line Element (TLE) epoch format to Unix timestamp
 *
 * TLE format stores epoch as: YYDDD.DDDDDDDD where:
 * - YY = 2-digit year (57-99 = 1957-1999, 00-56 = 2000-2056)
 * - DDD.DDDDDDDD = day of year with fractional part
 *
 * @param l1 First line of TLE containing epoch data in positions 19-32
 * @return Unix timestamp in seconds (double precision for subsecond accuracy)
 */
static inline double
TleEpochToUnixSeconds(const std::string& l1)
{
  // TLE columns are 1-based: [19-20]=YY, [21-32]=DDD.DDD...
  // Zero-based substr: start at 18 (2 chars), then 20 (12 chars)
  int yy = std::stoi(l1.substr(18, 2));
  double day = std::stod(l1.substr(20, 12)); // DDD.dddddddd

  // Convert YY to full year (TLE epoch rule: 57�99 -> 1957�1999, 00�56 ->
  // 2000�2056)
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
  static float fpsHistory[60] = {};
  static int fpsIndex = 0;

  ImGuiIO& io = ImGui::GetIO();
  fpsHistory[fpsIndex++ % 60] = io.Framerate;

  float avgFps = 0.0f;
  for (float f : fpsHistory)
    avgFps += f;
  avgFps /= 60.0f;

  auto* satelliteLayer = m_Scene->SatLayer();

  ImGui::Begin("Satellite Tracker");

  if (ImGui::CollapsingHeader("Performance")) {
    ImGui::Text("FPS: %.1f", avgFps);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / avgFps);
  }

  if (ImGui::CollapsingHeader("Simulation")) {
    ImGui::Text("Satellites: %zu", satelliteLayer->satellites().size());
    static float multiplier =
      static_cast<float>(m_Scene->state().satelliteSpeedMultiplier);
    if (ImGui::SliderFloat("Speed Multiplier", &multiplier, 1.f, 1500.f)) {
      m_Scene->state().satelliteSpeedMultiplier =
        static_cast<double>(multiplier);
    }
  }

  if (ImGui::CollapsingHeader("Visuals")) {
    ImGui::ColorEdit4("Background", &m_Renderer->clearColor().x);

    if (ImGui::SliderFloat("Near", &m_Ctx.satOptions.nearDistance, 0.f, 1000.f))
      satelliteLayer->MarkUniformsDirty();

    if (ImGui::SliderFloat("Far", &m_Ctx.satOptions.farDistance, 0.f, 1000.f))
      satelliteLayer->MarkUniformsDirty();

    if (ImGui::SliderFloat("Min Size", &m_Ctx.satOptions.minSize, 0.f, 50.f))
      satelliteLayer->MarkUniformsDirty();

    if (ImGui::SliderFloat("Max Size", &m_Ctx.satOptions.maxSize, 0.f, 100.f))
      satelliteLayer->MarkUniformsDirty();

    ImVec4 def = ToImVec4(m_Ctx.satOptions.defaultColor);
    ImVec4 hov = ToImVec4(m_Ctx.satOptions.hoverColor);
    ImVec4 sel = ToImVec4(m_Ctx.satOptions.selectColor);

    if (ImGui::ColorEdit3("Default", &def.x))
      m_Ctx.satOptions.defaultColor = FromImVec4(def);
    if (ImGui::ColorEdit3("Hover", &hov.x))
      m_Ctx.satOptions.hoverColor = FromImVec4(hov);
    if (ImGui::ColorEdit3("Selected", &sel.x))
      m_Ctx.satOptions.selectColor = FromImVec4(sel);
  }

  ImGui::End();
}

void
SatelliteTracker::FetchSatellitesData()
{
  TK_INFO("Loading satellites...");
  m_FetchingData = true;

#ifdef TERRAKIT_MOCK
  std::string m_TLEFilePath = m_Ctx.mockDataPath + "satelliteData.txt";

  std::ifstream file(m_TLEFilePath);
  if (!file.is_open()) {
    TK_ERROR(std::string("Cannot open local TLE file: ") + m_TLEFilePath);
    m_FetchingData = false;
    return;
  }
  std::string body((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  file.close();

  ParseTLEBody(body);
  m_FetchingData = false;
  TK_INFO("Satellites loaded from file");

  return;
#endif // TERRAKIT_MOCK

  static const std::string allSatellites =
    "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";

  Fetch::Get(
    allSatellites,
    [this](const std::string& body) {
      ParseTLEBody(body);
      m_FetchingData = false;
      TK_INFO("Satellites loaded from network");
    },
    [](const std::string& error) {
      TK_ERROR(std::string("Network error: ") + error);
    });
}
void
SatelliteTracker::ParseTLEBody(const std::string& body)
{
  std::stringstream ss(body);
  std::string line;
  int subLine = 0;
  std::string name, l1, l2;

  constexpr double mu = 3.986004418e14; // [m^3/s^2]

  while (std::getline(ss, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();

    if (subLine == 0) {
      name = line;
    } else if (subLine == 1) {
      l1 = line;
    } else if (subLine == 2) {
      l2 = line;

      Satellite::Orbit orbit{};
      OrbitWithName orbitWithName{};

      double i = std::stod(l2.substr(8, 8));
      double raan = std::stod(l2.substr(17, 8));
      double ecc = std::stod(l2.substr(26, 7));
      double w = std::stod(l2.substr(34, 8));
      double M0 = std::stod(l2.substr(43, 8));
      double n = std::stod(l2.substr(52, 11));

      orbit.inclination = glm::radians(i);
      orbit.raan = glm::radians(raan);
      orbit.eccentricity = ecc * 1e-7;
      orbit.argPerigee = glm::radians(w);
      orbit.meanAnomalyAtEpoch = glm::radians(M0);

      double n_rad = n * 2.0 * M_PI / 86400.0;
      orbit.semiMajorAxis = cbrt(mu / (n_rad * n_rad));

      orbit.epoch = TleEpochToUnixSeconds(l1);

      orbitWithName.orbit = std::move(orbit);
      orbitWithName.name = name;
      m_OrbitQueue.push(std::move(orbitWithName));
    }

    subLine = (subLine + 1) % 3;
  }
}