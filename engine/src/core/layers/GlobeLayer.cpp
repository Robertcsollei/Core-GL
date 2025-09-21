#pragma once
#include <terrakit/core/layers/GlobeLayer.h>

using namespace terrakit::core;

layers::GlobeLayer::GlobeLayer(AppContext& ctx)
  : m_Globe(ctx)
{
}

void
layers::GlobeLayer::Render(terrakit::renderer::Renderer& renderer,
                           const SceneState&)
{
  renderer.Submit(m_Globe.renderTask());
  renderer.Submit(m_Globe.atmosphereTask());
}
