#pragma once
#include "core/entities/Renderable.h"
#include "core/layers/Layer.h"
#include <memory>

namespace layers {
class GlobeLayer final : public ILayer
{
public:
  explicit GlobeLayer(std::unique_ptr<Renderable> globe)
    : m_Globe(std::move(globe))
  {
  }

  void render(Renderer& renderer, const FrameCtx&) override
  {
    if (m_Globe) {
      renderer.Submit(*m_Globe);
    }
  }

private:
  std::unique_ptr<Renderable> m_Globe;
};
}