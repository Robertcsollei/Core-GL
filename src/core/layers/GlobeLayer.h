#pragma once
#include "core/entities/Renderable.h"
#include "core/layers/Layer.h"
#include <core/entities/objects/Globe.h>
#include <memory>

namespace layers {
class GlobeLayer final : public ILayer
{
public:
  explicit GlobeLayer(AppContext& ctx)
    : m_Globe(ctx)
  {
  }
  ~GlobeLayer() = default;

  void OnAttach(SceneState&, AppContext&) override { m_Attached = true; }

  void Render(Renderer& renderer, const SceneState&) override
  {
    renderer.Submit(m_Globe.RenderTask());
  }

public:
  Globe& globe() { return m_Globe; }

private:
  Globe m_Globe;
};
}