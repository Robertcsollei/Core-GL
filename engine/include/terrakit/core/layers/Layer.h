#pragma once
#include <terrakit/core/AppContext.h>
#include <terrakit/core/SceneState.h>

namespace terrakit::core::config {
class RenderContext;
}

namespace terrakit::core {
namespace layers {

struct ILayer
{
public:
  virtual ~ILayer() = default;
  virtual void OnAttach(SceneState& state, AppContext& ctx) = 0;
  virtual void Render(terrakit::core::config::RenderContext& renderCtx,
                      const SceneState& state) = 0;
  virtual void Update(double dt, SceneState& state){};

protected:
  bool m_Attached = false;
};
} // namespace layers
} // namespace terrakit::core
