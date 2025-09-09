#pragma once
#include "core/entities/Renderable.h"
#include "core/layers/Layer.h"
#include <core/entities/Mesh.h>
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

  void OnAttach(SceneState&, AppContext&) override
  {
    m_Instances.resize(m_Satellites.size());
    m_Attached = true;
  }

  void Add(std::unique_ptr<Satellite> satellite)
  {
    m_Satellites.push_back(std::move(satellite));
    if (m_Attached) {
      m_Instances.emplace_back(); // grow the parallel buffer
    }
  };

  void Render(Renderer& renderer, const SceneState&) override
  {
    const size_t N = m_Satellites.size();
    for (size_t i = 0; i < N; ++i) {
      auto& s = *m_Satellites[i];
      m_Instances[i].Position = s.renderPos();
      m_Instances[i].Color = s.color();
    }
    auto& sharedPointMesh = m_Satellites.front()->RenderTask()->mesh;
    auto& pointShader = m_Satellites.front()->RenderTask()->material->shader;
    renderer.SubmitPointsInstanced(
      sharedPointMesh, pointShader.get(), m_Instances);
  }

  void Update(double dt) override
  {
    static bool once = false;
    for (auto& sat : m_Satellites) {
      if (!once)
        sat->Update(dt);
    }

    once = true;
  }

public:
  const std::vector<std::unique_ptr<Satellite>>& satellites() const
  {
    return m_Satellites;
  }

private:
  std::vector<std::unique_ptr<Satellite>> m_Satellites;
  std::vector<Mesh::PointVertex> m_Instances;
};
}