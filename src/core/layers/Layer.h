#pragma once
#include "Renderer.h"
#include <core/AppContext.h>
#include <core/SceneState.h>

namespace layers {

struct ILayer
{
public:
  virtual ~ILayer() = default;
  virtual void OnAttach(SceneState& state, AppContext& ctx) = 0;
  virtual void Render(Renderer& R, const SceneState& state) = 0;
  virtual void Update(double dt){};

protected:
  bool m_Attached = false;
};
}
