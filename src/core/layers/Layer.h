#pragma once
#include "Renderer.h"
#include "core/Scene.h"

namespace layers {
struct FrameCtx
{
  const SceneState& state;
  const Camera& camera;
};

struct ILayer
{
  virtual ~ILayer() = default;
  virtual void render(Renderer& R, const FrameCtx& ctx) = 0;
};
}
