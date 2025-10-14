#pragma once
#include <SDL.h>
#include <terrakit/core/AppContext.h>
#include <terrakit/core/entities/Camera.h>

namespace terrakit::core {
class CameraControls
{
public:
  explicit CameraControls(Camera& cam, AppContext& ctx)
    : m_Camera(cam)
    , m_Ctx(ctx)
  {
  }
  ~CameraControls() = default;

  inline void Attach(Camera& camera) { m_Camera = camera; }
  void HandleEvent(const SDL_Event& event, AppContext& ctx);

public:
  Camera& camera() { return m_Camera; }

  float rotateDegPerPixel = 0.25f;
  float zoomStep = 0.90f;
  float panScale = 1.0f;

private:
  // Touch gesture handlers
  void HandleSingleFingerDown(const SDL_Event& e, AppContext& ctx);
  void HandleSingleFingerUp(const SDL_Event& e, AppContext& ctx);
  void HandleSingleFingerMotion(const SDL_Event& e, AppContext& ctx);
  void HandlePinchGesture(const SDL_Event& e, AppContext& ctx);

  // Mouse handlers
  void HandleMouseButtonDown(const SDL_Event& e, AppContext& ctx);
  void HandleMouseButtonUp(const SDL_Event& e, AppContext& ctx);
  void HandleMouseMotion(const SDL_Event& e, AppContext& ctx);
  void HandleMouseWheel(const SDL_Event& e, AppContext& ctx);

  // Camera operations
  void RotateCamera(int dx, int dy);
  void PanCamera(int dx, int dy);
  void ZoomCamera(float factor);

private:
  Camera& m_Camera;
  AppContext& m_Ctx;

  static constexpr float kModeSwapTreshold = 200.f;

  bool m_Rotating = false;
  bool m_Panning = false;
  int m_LastX = 0, m_LastY = 0;
};
} // namespace terrakit::core