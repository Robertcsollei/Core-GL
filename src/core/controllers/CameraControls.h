#pragma once
#include "core/AppContext.h"
#include "core/entities/camera/Camera.h"
#include <SDL.h>

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

  Camera& camera() { return m_Camera; }

  float rotateDegPerPixel = 0.25f;
  float zoomStep = 0.90f;
  float panScale = 1.0f;

private:
  Camera& m_Camera;
  AppContext& m_Ctx;

  static constexpr float kModeSwapTreshold = 200.f;

  bool m_Rotating = false;
  bool m_Panning = false;
  int m_LastX = 0, m_LastY = 0;
};