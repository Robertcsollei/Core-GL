#pragma once
#include "core/entities/Renderable.h"
#include "core/layers/Layer.h"
#include <core/entities/objects/Globe.h>
#include <core/entities/objects/Satellite.h>
#include <memory>

namespace layers {
class SatelliteLayer final : public ILayer
{
public:
  explicit SatelliteLayer(AppContext& ctx)
    : m_Satellites()
  {
  }
  ~SatelliteLayer() = default;

  void OnAttach(SceneState&, AppContext&) override {}

  void Add(std::unique_ptr<Satellite> satellite)
  {
    m_Satellites.push_back(std::move(satellite));
  };

  void Render(Renderer& renderer, const SceneState&) override
  {
    std::vector<glm::vec3> positions;
    positions.reserve(m_Satellites.size());
    for (auto& sat : m_Satellites) {
      positions.push_back(glm::vec3(sat->position()));
    }
    auto& sharedPointMesh = m_Satellites.front()->RenderTask()->mesh;
    auto& pointShader = m_Satellites.front()->RenderTask()->material->shader;
    renderer.SubmitPointsInstanced(
      sharedPointMesh, pointShader.get(), positions);
  }

  void Update(double dt) override
  {
    for (auto& sat : m_Satellites) {
      sat->Update(dt);
    }
  }

public:
  const std::vector<std::unique_ptr<Satellite>>& satellites() const
  {
    return m_Satellites;
  }

private:
  std::vector<std::unique_ptr<Satellite>> m_Satellites;
};
}