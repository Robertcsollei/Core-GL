#include <terrakit/core/controllers/CameraControls.h>

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

namespace terrakit::core {

void
CameraControls::HandleEvent(const SDL_Event& e, AppContext& ctx)
{
  const auto& io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;

  switch (e.type) {
    case SDL_FINGERDOWN:
      HandleSingleFingerDown(e, ctx);
      break;
    case SDL_FINGERUP:
      HandleSingleFingerUp(e, ctx);
      break;
    case SDL_FINGERMOTION:
      HandleSingleFingerMotion(e, ctx);
      break;
    case SDL_MULTIGESTURE:
      HandlePinchGesture(e, ctx);
      break;
    case SDL_MOUSEBUTTONDOWN:
      HandleMouseButtonDown(e, ctx);
      break;
    case SDL_MOUSEBUTTONUP:
      HandleMouseButtonUp(e, ctx);
      break;
    case SDL_MOUSEMOTION:
      HandleMouseMotion(e, ctx);
      break;
    case SDL_MOUSEWHEEL:
      HandleMouseWheel(e, ctx);
      break;
    default:
      break;
  }
}

// ============================================================================
// Touch Gesture Handlers
// ============================================================================

void
CameraControls::HandleSingleFingerDown(const SDL_Event& e, AppContext& ctx)
{
  m_Rotating = true;
  m_LastX = static_cast<int>(e.tfinger.x * ctx.width);
  m_LastY = static_cast<int>(e.tfinger.y * ctx.height);
}

void
CameraControls::HandleSingleFingerUp(const SDL_Event& e, AppContext& ctx)
{
  m_Rotating = false;
  m_Panning = false;
  ctx.pointerLock = false;
}

void
CameraControls::HandleSingleFingerMotion(const SDL_Event& e, AppContext& ctx)
{
  // Don't rotate if we're in the middle of a multi-touch gesture
  if (!m_Rotating)
    return;

  int currentX = static_cast<int>(e.tfinger.x * ctx.width);
  int currentY = static_cast<int>(e.tfinger.y * ctx.height);

  int dx = currentX - m_LastX;
  int dy = currentY - m_LastY;

  m_LastX = currentX;
  m_LastY = currentY;

  // Single finger = rotate camera
  RotateCamera(dx, dy);
}

void
CameraControls::HandlePinchGesture(const SDL_Event& e, AppContext& ctx)
{
  // Only handle 2-finger gestures
  if (e.mgesture.numFingers != 2)
    return;

  // Disable rotation during pinch to prevent conflicts
  m_Rotating = false;

  // Pinch = distance change between fingers
  float pinchDelta = e.mgesture.dDist;

  // Only zoom if there's a meaningful pinch
  if (std::abs(pinchDelta) > 0.001f) {
    // Convert pinch delta to zoom factor
    // Negative dDist = pinch in = zoom in
    float factor = 1.0f + (pinchDelta * 2.0f);
    ZoomCamera(factor);
  }
}

// ============================================================================
// Mouse Handlers
// ============================================================================

void
CameraControls::HandleMouseButtonDown(const SDL_Event& e, AppContext& ctx)
{
  if (e.button.button == SDL_BUTTON_LEFT) {
    m_Rotating = true;
    m_LastX = e.button.x;
    m_LastY = e.button.y;
  } else if (e.button.button == SDL_BUTTON_MIDDLE ||
             (e.button.button == SDL_BUTTON_RIGHT &&
              (SDL_GetModState() & KMOD_SHIFT))) {
    m_Panning = true;
    m_LastX = e.button.x;
    m_LastY = e.button.y;
  }
}

void
CameraControls::HandleMouseButtonUp(const SDL_Event& e, AppContext& ctx)
{
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
}

void
CameraControls::HandleMouseMotion(const SDL_Event& e, AppContext& ctx)
{
  if (!m_Rotating && !m_Panning)
    return;

  ctx.pointerLock = true;

  int dx = e.motion.x - m_LastX;
  int dy = e.motion.y - m_LastY;
  m_LastX = e.motion.x;
  m_LastY = e.motion.y;

  if (m_Rotating) {
    RotateCamera(dx, dy);
  }
  if (m_Panning) {
    PanCamera(dx, dy);
  }
}

void
CameraControls::HandleMouseWheel(const SDL_Event& e, AppContext& ctx)
{
  float delta =
    (e.wheel.preciseY != 0.0f) ? e.wheel.preciseY : (float)e.wheel.y;

  float factor = (delta > 0.0f) ? zoomStep : (1.0f / zoomStep);
  ZoomCamera(factor);
}

// ============================================================================
// Camera Operations
// ============================================================================

void
CameraControls::RotateCamera(int dx, int dy)
{
  m_Camera.AddOrbitDelta(dx * -rotateDegPerPixel, dy * rotateDegPerPixel);
}

void
CameraControls::PanCamera(int dx, int dy)
{
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

void
CameraControls::ZoomCamera(float factor)
{
  float currentDist = m_Camera.distance();
  float newDist = std::clamp(currentDist / factor, 0.1f, 100000.0f);

  // Only change distance, preserve current orientation
  // This prevents flickering when rapid pinch events arrive
  float currentYaw = m_Camera.yawDeg();
  float currentPitch = m_Camera.pitchDeg();
  m_Camera.SetOrbit(newDist, currentYaw, currentPitch);
}

} // namespace terrakit::core