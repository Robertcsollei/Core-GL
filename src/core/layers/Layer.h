#pragma once
#include <core/AppContext.h>
#include <core/SceneState.h>
#include <renderer/Renderer.h>

namespace layers {

struct ILayer
{
public:
  virtual ~ILayer() = default;
  virtual void OnAttach(SceneState& state, AppContext& ctx) = 0;
  virtual void Render(Renderer& R, const SceneState& state) = 0;
  virtual void Update(double dt, SceneState& state){};

protected:
  bool m_Attached = false;
};
}
