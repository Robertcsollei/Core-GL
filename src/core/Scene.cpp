#include "core/Scene.h"
#include "Renderer.h"
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
}

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
    layer->Update(m_SimTime);
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
  // for (auto& layer : m_Layers) {
  //   if (auto* satLayer = dynamic_cast<layers::SatelliteLayer*>(layer.get()))
  //   {
  //     for (auto& sat : satLayer->satellites()) {
  //       ImGui::DragScalar("Epoch", ImGuiDataType_Double, sat->epoch(), 1.0);
  //     }
  //   }
  // }
  ImGuiIO& io = ImGui::GetIO();
  ImGui::Text("FPS: %.1f", io.Framerate);
  ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
  ImGui::End();
}

void
Scene::HandleEvent(const SDL_Event& e)
{
  m_CamCtl.HandleEvent(e);
}

void
Scene::InitLayers()
{
  auto globeLayer = std::make_unique<layers::GlobeLayer>(m_Ctx);
  auto& globe = globeLayer->globe();
  AddLayer(std::move(globeLayer));

  auto satelliteLayer = std::make_unique<layers::SatelliteLayer>(m_Ctx);

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

    auto sat = std::make_unique<Satellite>(m_Ctx, orbit, 0.0, globe);
    satelliteLayer->Add(std::move(sat));
  }

  AddLayer(std::move(satelliteLayer));
}

void
Scene::UpdateCameraDefault()
{
  m_State.camera.setPerspective(
    45.f, m_Ctx.width / m_Ctx.height, 0.1f, 10000.f);
  m_State.camera.setLookAt({ 0, 0, 300 }, { 0, 0, -800 }, { 0, 1, 0 });
}
