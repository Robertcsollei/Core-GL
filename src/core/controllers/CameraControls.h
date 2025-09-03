#pragma once
#include "core/entities/camera/Camera.h"
#include <SDL.h>

class CameraControls
{
public:
  explicit CameraControls(Camera* cam)
    : m_Camera(cam)
  {
  }
  ~CameraControls() = default;

  void SetCamera(Camera* camera) { m_Camera = camera; }
  void Update(const SDL_Event& event);

  Camera* camera() { return m_Camera; }

  float rotateDegPerPixel = 0.25f;
  float zoomStep = 0.90f;
  float panScale = 1.0f;

private:
  Camera* m_Camera = nullptr;

  static constexpr float kModeSwapTreshold = 200.f;

  int m_VPW = 1280, m_VPH = 720;
  bool m_Rotating = false;
  bool m_Panning = false;
  int m_LastX = 0, m_LastY = 0;
};