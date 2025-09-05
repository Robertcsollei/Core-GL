#pragma once
#include "Renderer.h"
#include <core/AppContext.h>
#include <core/Scene.h>
#include <core/SceneState.h>

namespace layers {

struct ILayer
{
  virtual ~ILayer() = default;
  virtual void OnAttach(SceneState& state, AppContext& ctx) = 0;
  virtual void Render(Renderer& R, const SceneState& state) = 0;
  virtual void Update(double dt){};
};
}
