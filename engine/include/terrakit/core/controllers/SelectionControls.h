#pragma once
#include <SDL.h>
#include <optional>
#include <terrakit/core/AppContext.h>
#include <terrakit/core/SceneState.h>

namespace terrakit::core {

class Satellite;

namespace layers {
class SatelliteLayer;
}
class Globe;

class SelectionControls
{
public:
  struct Selection
  {
    Satellite* satellite;
    glm::ivec2 screenPos;

    Selection()
      : satellite(nullptr)
      , screenPos(-1, -1)
    {
    }

    Selection(Satellite* sat, glm::ivec2 sPos)
      : satellite(sat)
      , screenPos(sPos)
    {
    }
  };

public:
  SelectionControls(layers::SatelliteLayer* lyr,
                    Globe& globe,
                    AppContext& ctx,
                    SceneState& state);
  ~SelectionControls() = default;

  void HandleEvent(const SDL_Event& event);
  void HandleHoverState();

  void UpdatePointerRay();
  void OnResize();

  void ClearSelection();

public:
  const Selection* hovered() const
  {
    return m_Hovered.has_value() ? &m_Hovered.value() : nullptr;
  }

private:
  glm::ivec2 m_MousePos;
  glm::dvec3 m_PointerRay;

  layers::SatelliteLayer* m_SatLayer;
  Globe& m_Globe;
  std::optional<Selection> m_Hovered;
  std::optional<Selection> m_Selected;

  AppContext& m_Ctx;
  SceneState& m_State;

private:
  bool IsVisibleFromCamera(const glm::dvec3& camPos, const glm::dvec3& satPos);
};
} // namespace terrakit::core