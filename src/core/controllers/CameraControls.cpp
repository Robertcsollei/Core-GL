#include "CameraControls.h"

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

#include <iostream>

void
CameraControls::HandleEvent(const SDL_Event& e, AppContext& ctx)
{
  const auto& io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;

  switch (e.type) {
    case SDL_MOUSEBUTTONDOWN:
      if (e.button.button == SDL_BUTTON_LEFT) {
        m_Rotating = true;
        m_LastX = e.button.x;
        m_LastY = e.button.y;
        ctx.pointerLock = true;
      } else if (e.button.button == SDL_BUTTON_MIDDLE ||
                 (e.button.button == SDL_BUTTON_RIGHT &&
                  (SDL_GetModState() & KMOD_SHIFT))) {
        m_Panning = true;
        m_LastX = e.button.x;
        m_LastY = e.button.y;
        ctx.pointerLock = true;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      if (e.button.button == SDL_BUTTON_LEFT) {
        m_Rotating = false;
        ctx.pointerLock = false;
      }
      if (e.button.button == SDL_BUTTON_MIDDLE ||
          (e.button.button == SDL_BUTTON_RIGHT &&
           (SDL_GetModState() & KMOD_SHIFT))) {
        m_Panning = false;
        ctx.pointerLock = false;
      }
      break;

    case SDL_MOUSEMOTION: {
      if (!ctx.pointerLock)
        break;

      int dx = e.motion.x - m_LastX;
      int dy = e.motion.y - m_LastY;
      m_LastX = e.motion.x;
      m_LastY = e.motion.y;

      if (m_Rotating) {
        m_Camera.AddOrbitDelta(dx * -rotateDegPerPixel, dy * rotateDegPerPixel);
      }
      if (m_Panning) {
        float dist = m_Camera.distance();
        float fovDeg = m_Camera.fovDeg();
        float worldPerPixel =
          2.0f * dist * std::tan(glm::radians(fovDeg * 0.5f)) / m_Ctx.height;

        glm::vec3 eye = m_Camera.position();
        glm::vec3 tgt = m_Camera.target();
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 fwd = glm::normalize(tgt - eye);
        glm::vec3 right = glm::normalize(glm::cross(fwd, up));
        glm::vec3 camUp = glm::normalize(glm::cross(right, fwd));

        glm::vec3 delta =
          (-right * float(dx) + camUp * float(dy)) * worldPerPixel * panScale;
        m_Camera.SetLookAtTarget(tgt + delta);
      }
      break;
    }

    case SDL_MOUSEWHEEL: {

      float step = (e.wheel.y > 0) ? zoomStep : (1.0f / zoomStep);
      float newDist = std::clamp(m_Camera.distance() * step, 0.1f, 100000.0f);

      m_Camera.SetOrbit(newDist, m_Camera.yawDeg(), m_Camera.pitchDeg());
      break;
    }

    default:
      break;
  }
}
