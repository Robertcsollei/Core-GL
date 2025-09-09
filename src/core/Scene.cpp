#include "Renderer.h"
#include <core/Scene.h>
#include <core/controllers/SelectionControls.h>
#include <core/entities/objects/Satellite.h>
#include <core/layers/GlobeLayer.h>
#include <core/layers/SatelliteLayer.h>
#include <imgui/imgui.h>
#include <memory>
#include <random>

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
  static bool once = false;
  m_SimTime += dt;
  for (auto& layer : m_Layers) {
    layer->Update(m_SimTime);
  }

  m_HoverTimer += dt;
  if (m_HoverTimer >= 0.15) {
    // m_SelectionCtl->HandleHoverState();
    m_HoverTimer = 0.0;
  }
}

void
Scene::Render(Renderer& renderer)
{
  for (auto& layer : m_Layers) {
    layer->Render(renderer, m_State);
  }
}

void
Scene::RenderUI(AppContext& ctx)
{
  ImGui::Begin("Test Popup");
  ImGui::Text("Hello World...");

  ImGuiIO& io = ImGui::GetIO();
  ImGui::Text("FPS: %.1f", io.Framerate);
  ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
  ImGui::End();

  if (auto sel = m_SelectionCtl->selection()) {
    ImGui::SetNextWindowPos(
      ImVec2((float)sel->screenPos.x, (float)sel->screenPos.y),
      ImGuiCond_Always,
      ImVec2(0.0f, 1.2f));
    ImGui::Begin(
      ("SatelliteTooltip##" + sel->uuid).c_str(),
      nullptr,
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

    ImGui::TextUnformatted(sel->uuid.c_str());
    ImGui::End();
  }
}

void
Scene::HandleEvent(const SDL_Event& e)
{
  m_CamCtl.HandleEvent(e, m_Ctx);
  m_SelectionCtl->HandleEvent(e);
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

  // Random number generators
  std::mt19937 rng{ std::random_device{}() };
  std::uniform_real_distribution<double> distInc(
    0.0, glm::pi<double>()); // inclination [0, 180°]
  std::uniform_real_distribution<double> distRaan(
    0.0, glm::two_pi<double>()); // RAAN [0, 360°]
  std::uniform_real_distribution<double> distArg(
    0.0, glm::two_pi<double>()); // arg of perigee
  std::uniform_real_distribution<double> distM0(
    0.0, glm::two_pi<double>()); // mean anomaly at epoch
  std::uniform_real_distribution<double> distAlt(
    400000.0, 1200000.0); // altitude [400–1200 km]

  for (int i = 0; i < 14000; i++) {
    Satellite::Orbit orbit{};
    orbit.semiMajorAxis = 6378137.0 + distAlt(rng); // Earth radius + altitude
    orbit.eccentricity = 0.0;
    orbit.inclination = distInc(rng);
    orbit.raan = distRaan(rng);
    orbit.argPerigee = distArg(rng);
    orbit.meanAnomalyAtEpoch = distM0(rng);
    orbit.epoch = 0.0;

    auto sat = std::make_unique<Satellite>(m_Ctx,
                                           "satellite_" + std::to_string(i),
                                           orbit,
                                           0.0,
                                           m_GlobeLayer->globe());
    satelliteLayer->Add(std::move(sat));
  }

  AddLayer(std::move(satelliteLayer));
}

void
Scene::UpdateCameraDefault()
{
  m_State.camera.SetPerspective(
    45.f, m_Ctx.width / m_Ctx.height, 0.1f, 10000.f);
  m_State.camera.SetLookAt({ 0, 0, 300 }, { 0, 0, -800 }, { 0, 1, 0 });
}
