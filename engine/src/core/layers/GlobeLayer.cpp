#include <terrakit/core/layers/GlobeLayer.h>
#include <terrakit/core/config/RenderContext.h>

using namespace terrakit::core;

layers::GlobeLayer::GlobeLayer(AppContext& ctx)
  : m_Globe(ctx)
{
}

void
layers::GlobeLayer::Render(config::RenderContext& renderCtx,
                           const SceneState&)
{
  renderCtx.Submit(m_Globe.renderTask());
  renderCtx.Submit(m_Globe.atmosphereTask());
}
