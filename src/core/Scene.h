#pragma once

#include <core/SceneState.h>
#include <core/controllers/CameraControls.h>
#include <core/layers/GlobeLayer.h>
#include <memory>
#include <vector>

class Scene
{
public:
  using LayerPtr = std::unique_ptr<layers::ILayer>;

  Scene(AppContext& ctx);

  void AddLayer(LayerPtr layer);
  void Update(double dt);
  void Render(Renderer& renderer);
  void RenderUI(AppContext& ctx);

  void HandleEvent(const SDL_Event& e);
  inline void OnResize() { m_State.camera.onResize(m_Ctx.width, m_Ctx.height); }

  const SceneState& state() const { return m_State; }

private:
  std::vector<LayerPtr> m_Layers;
  CameraControls m_CamCtl;
  AppContext& m_Ctx;
  SceneState m_State;
  double m_SimTime = 0.0;

private:
  void InitLayers();
  void UpdateCameraDefault();
};