#include <imgui/imgui.h>
#include <memory>
#include <terrakit/core/Scene.h>
#include <terrakit/core/config/RenderContext.h>
#include <terrakit/core/controllers/SelectionControls.h>
#include <terrakit/core/entities/Satellite.h>
#include <terrakit/core/layers/GlobeLayer.h>
#include <terrakit/core/layers/SatelliteLayer.h>

namespace terrakit::core {

Scene::Scene(AppContext& ctx)
  : m_State()
  , m_Ctx(ctx)
  , m_CamCtl(m_State.camera, ctx)
{
  UpdateCameraDefault();
  InitLayers();
  m_SelectionCtl = std::make_unique<SelectionControls>(
    m_SatLayer, m_GlobeLayer->globe(), m_Ctx, m_State);
}

Scene::~Scene() = default;

void
Scene::AddLayer(LayerPtr layer)
{
  layer->OnAttach(m_State, m_Ctx);
  m_Layers.emplace_back(std::move(layer));
}

void
Scene::Update(double dt)
{
  m_SimTime += dt;
  for (auto& layer : m_Layers) {
    layer->Update(m_SimTime, m_State);
  }

  m_HoverTimer += dt;
  if (m_HoverTimer >= 0.15) {
    m_SelectionCtl->HandleHoverState();
    m_HoverTimer = 0.0;
  }
}

void
Scene::Render(config::RenderContext& renderCtx)
{
  for (auto& layer : m_Layers) {
    layer->Render(renderCtx, m_State);
  }
}

void
Scene::RenderUI(AppContext& ctx)
{
  if (auto hovered = m_SelectionCtl->hovered()) {
    ImGui::SetNextWindowPos(
      ImVec2((float)hovered->screenPos.x, (float)hovered->screenPos.y),
      ImGuiCond_Always,
      ImVec2(0.0f, 1.2f));
    ImGui::Begin(
      ("SatelliteTooltip##" + hovered->satellite->uuid()).c_str(),
      nullptr,
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

    ImGui::TextUnformatted(hovered->satellite->uuid().c_str());
    ImGui::End();
  }
}

void
Scene::HandleEvent(const SDL_Event& e)
{
  m_SelectionCtl->HandleEvent(e);
  m_CamCtl.HandleEvent(e, m_Ctx);
}

void
Scene::OnResize()
{
  m_State.camera.OnResize(m_Ctx.width, m_Ctx.height);
  m_SelectionCtl->OnResize();
}

void
Scene::InitLayers()
{
  auto globeLayer = std::make_unique<layers::GlobeLayer>(m_Ctx);
  m_GlobeLayer = globeLayer.get();
  AddLayer(std::move(globeLayer));

  auto satelliteLayer = std::make_unique<layers::SatelliteLayer>(m_Ctx);
  m_SatLayer = satelliteLayer.get();

  AddLayer(std::move(satelliteLayer));
}

void
Scene::UpdateCameraDefault()
{
  m_State.camera.SetPerspective(
    45.f, m_Ctx.width / m_Ctx.height, 0.1f, 10000.f);
  m_State.camera.SetLookAt({ 0, 0, 300 }, { 0, 0, -800 }, { 0, 1, 0 });
}
}