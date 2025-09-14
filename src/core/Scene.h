#pragma once

#include <core/SceneState.h>
#include <core/controllers/CameraControls.h>
#include <core/layers/Layer.h>
#include <memory>
#include <renderer/Renderer.h>
#include <vector>

namespace layers {
class SatelliteLayer;
class GlobeLayer;
}
class SelectionControls;

class Scene
{
public:
  using LayerPtr = std::unique_ptr<layers::ILayer>;

  Scene(AppContext& ctx);
  ~Scene();

  void AddLayer(LayerPtr layer);
  void Update(double dt);
  void Render(Renderer& renderer);
  void RenderUI(AppContext& ctx);

  void HandleEvent(const SDL_Event& e);
  void OnResize();

  SceneState& state() { return m_State; }
  layers::SatelliteLayer* SatLayer() { return m_SatLayer; }
  layers::GlobeLayer* GlobeLayer() { return m_GlobeLayer; }

private:
  std::vector<LayerPtr> m_Layers;
  layers::SatelliteLayer* m_SatLayer = nullptr;
  layers::GlobeLayer* m_GlobeLayer = nullptr;

  CameraControls m_CamCtl;
  std::unique_ptr<SelectionControls> m_SelectionCtl;
  AppContext& m_Ctx;
  SceneState m_State;
  double m_SimTime = 0.0;
  double m_HoverTimer = 0.0;

private:
  void InitLayers();
  void UpdateCameraDefault();
};