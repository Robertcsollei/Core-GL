#include <SDL.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <terrakit/core/AppContext.h>
#include <terrakit/core/controllers/SelectionControls.h>
#include <terrakit/core/entities/Globe.h>
#include <terrakit/core/layers/SatelliteLayer.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <terrakit/core/Mesh.h>
#include <terrakit/core/entities/Satellite.h>

#include <iostream>

using namespace terrakit::core;

static double
GetSatellitePointSize(SceneState& state, const glm::dvec3& satPos)
{
  auto& camera = state.camera;
  glm::dvec4 eyePos = camera.view() * glm::dvec4(satPos, 1.0);
  double dist = glm::length(glm::dvec3(eyePos));

  // Defined in Point.shader originally.
  static const double maxSize = 20.0;
  static const double minSize = 4.0;
  static const double nearDist = 20.0;
  static const double farDist = 200.0;

  double t = glm::clamp((dist - nearDist) / (farDist - nearDist), 0.0, 1.0);
  double pointSizePx = glm::mix(maxSize, minSize, t);
  return pointSizePx;
}

SelectionControls::SelectionControls(layers::SatelliteLayer* lyr,
                                     Globe& globe,
                                     AppContext& ctx,
                                     SceneState& state)
  : m_SatLayer(lyr)
  , m_Globe(globe)
  , m_MousePos(0)
  , m_PointerRay(0.0)
  , m_Ctx(ctx)
  , m_State(state)

{
}

void
SelectionControls::HandleEvent(const SDL_Event& e)
{

  const auto& io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
    m_Hovered.reset();
    return;
  }

  switch (e.type) {

    case SDL_MOUSEBUTTONDOWN:
      break;

    case SDL_MOUSEBUTTONUP:

      if (m_Ctx.pointerLock)
        return;

      switch (e.button.button) {
        case SDL_BUTTON_LEFT:
          if (m_Hovered) {
            if (m_Selected) {
              m_Selected->satellite->state.active = false;
            }
            // TODO: maintain only one selection
            m_Selected.emplace(m_Hovered->satellite, m_Hovered->screenPos);
            m_Selected->satellite->state.active = true;
            m_State.selectedSat = m_Hovered->satellite;
          } else {
            ClearSelection();
          }

          break;
      }

      break;

    case SDL_MOUSEMOTION: {
      if (m_Ctx.pointerLock) {
        m_Hovered.reset();
        return;
      }

      m_MousePos = glm::ivec2(e.motion.x, e.motion.y);
      UpdatePointerRay();

      break;
    }

    default:
      break;
  }
}

void
SelectionControls::HandleHoverState()
{
  if (m_Ctx.pointerLock) {
    return;
  }

  glm::dvec3 camPos = glm::dvec3(m_State.camera.position());

  for (auto& sat : m_SatLayer->satellites()) {
    auto& satPos = sat->position();
    glm::dvec3 camToSat = glm::normalize(satPos - camPos);
    double cosAngle = glm::dot(camToSat, m_PointerRay);

    if (cosAngle < 1.0 - 1e-2 || !IsVisibleFromCamera(camPos, satPos)) {
      sat->state.hovered = false;
      if (m_Hovered.has_value() &&
          m_Hovered->satellite->uuid() == sat->uuid()) {
        m_Hovered.reset();
      }
      continue;
    }

    glm::ivec2 satScreenPos =
      m_State.camera.WorldToScreen(satPos, m_Ctx.width, m_Ctx.height);

    if (satScreenPos.x - 10 > m_Ctx.width || satScreenPos.x + 10 < 0 ||
        satScreenPos.y - 10 > m_Ctx.height || satScreenPos.y + 10 < 0) {
      continue;
    }

    glm::vec2 d = m_MousePos - satScreenPos;
    double dist2 = glm::dot(d, d);

    double pointSizePx = GetSatellitePointSize(m_State, satPos);
    double radiusPx = pointSizePx * 0.5;

    if (dist2 <= radiusPx * radiusPx) {
      if (m_Hovered)
        m_Hovered->satellite->state.hovered = false;

      m_Hovered.emplace(sat.get(), satScreenPos);
      sat->state.hovered = true;
    } else {
      sat->state.hovered = false;
      if (m_Hovered.has_value() &&
          m_Hovered->satellite->uuid() == sat->uuid()) {
        m_Hovered.reset();
      }
    }
  }
}

void
SelectionControls::UpdatePointerRay()
{
  double ndcX = (2.0 * m_MousePos.x) / m_Ctx.width - 1.0;
  double ndcY = 1.0 - (2.0 * m_MousePos.y) / m_Ctx.height;

  m_PointerRay = m_State.camera.CastRay(ndcX, ndcY);
}

void
SelectionControls::OnResize()
{
  UpdatePointerRay();
}

void
SelectionControls::ClearSelection()
{
  if (m_Selected)
    m_Selected->satellite->state.active = false;
  m_Selected.reset();
  m_State.selectedSat = nullptr;
}

bool
SelectionControls::IsVisibleFromCamera(const glm::dvec3& camPos,
                                       const glm::dvec3& satPos)
{
  glm::dvec3 d = satPos - camPos;
  glm::dvec3 f =
    camPos - glm::dvec3(m_Globe.RenderTask()->transform.translation);

  double globeRadius = m_Globe.Geometry().Radii().x;
  double a = glm::dot(d, d);
  double b = 2.0 * glm::dot(f, d);
  double c = glm::dot(f, f) - globeRadius * globeRadius;

  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0.0) {
    return true; // no intersection
  }

  discriminant = std::sqrt(discriminant);
  double t0 = (-b - discriminant) / (2 * a);
  double t1 = (-b + discriminant) / (2 * a);

  if ((t0 >= 0.0 && t0 <= 1.0) || (t1 >= 0.0 && t1 <= 1.0)) {
    return false; // intersects Earth between camera and sat
  }
  return true;
}