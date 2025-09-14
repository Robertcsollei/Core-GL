#include <core/geometry/ObjectFactory.h>
#include <core/layers/SatelliteLayer.h>

void
layers::SatelliteLayer::OnAttach(SceneState&, AppContext&)
{
  m_Instances.resize(m_Satellites.size());
  m_Attached = true;
}

void
layers::SatelliteLayer::Add(std::unique_ptr<Satellite> satellite)
{
  m_Satellites.push_back(std::move(satellite));
  if (m_Attached) {
    m_Instances.emplace_back(); // grow the parallel buffer
  }
};

void
layers::SatelliteLayer::Render(Renderer& renderer, const SceneState&)
{
  if (m_Satellites.empty() || m_Paused)
    return;

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

  if (m_SelectedOrbit.has_value()) {
    renderer.SubmitLine(&m_SelectedOrbit->orbitPath);
  }
}

void
layers::SatelliteLayer::Update(double dt, SceneState& state)
{
  if (m_Paused)
    return;

  if (state.selectedSat) {
    if (!m_SelectedOrbit || m_SelectedOrbit->sat != state.selectedSat) {
      m_SelectedOrbit.emplace(
        state.selectedSat,
        ObjectFactory::CreateOrbit(
          state.selectedSat->uuid(), dt, glm::vec3(0.f), state, m_Ctx));
    }
  } else {
    m_SelectedOrbit.reset();
  }

  for (auto& sat : m_Satellites) {
    sat->Update(dt, state);
  }
}
