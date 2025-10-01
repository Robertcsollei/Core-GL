#pragma once
#include <memory>
#include <terrakit/core/Renderable.h>
#include <terrakit/core/entities/Globe.h>
#include <terrakit/core/layers/Layer.h>

namespace terrakit::core {
namespace layers {
class GlobeLayer final : public ILayer
{
public:
  GlobeLayer(AppContext& ctx);
  ~GlobeLayer() = default;

  void OnAttach(SceneState&, AppContext&) override { m_Attached = true; }

  void Render(terrakit::core::config::RenderContext& renderCtx,
              const SceneState&) override;

public:
  Globe& globe() { return m_Globe; }

private:
  Globe m_Globe;
};
} // namespace layers
} // namespace terrakit::core