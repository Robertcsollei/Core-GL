#pragma once
#include <core/entities/Mesh.h>
#include <core/entities/Renderable.h>
#include <core/entities/objects/Globe.h>
#include <core/entities/objects/Satellite.h>
#include <core/layers/Layer.h>
#include <memory>
#include <optional>

namespace layers {
class SatelliteLayer final : public ILayer
{
public:
  struct SelectedOrbit
  {
    Satellite* sat;
    Renderable orbitPath;
  };

public:
  explicit SatelliteLayer(AppContext& ctx)
    : m_Ctx(ctx)
    , m_Satellites()
  {
  }
  ~SatelliteLayer() = default;

  void OnAttach(SceneState&, AppContext&) override;
  void Add(std::unique_ptr<Satellite> satellite);
  void Render(Renderer& renderer, const SceneState& state) override;
  void Update(double dt, SceneState& state) override;

  void Pause() { m_Paused = true; }
  void Resume() { m_Paused = false; }

public:
  const std::vector<std::unique_ptr<Satellite>>& satellites() const
  {
    return m_Satellites;
  }

private:
  AppContext& m_Ctx;

  std::vector<std::unique_ptr<Satellite>> m_Satellites;
  std::vector<Mesh::PointVertex> m_Instances;
  std::optional<SelectedOrbit> m_SelectedOrbit;

  bool m_Paused = false;
};
}