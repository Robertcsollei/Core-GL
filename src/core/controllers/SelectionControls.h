#pragma once
#include "core/AppContext.h"
#include <SDL.h>
#include <core/SceneState.h>
#include <optional>

namespace layers {
class SatelliteLayer;
}
class Globe;

class SelectionControls
{
public:
  struct Selection
  {
    const std::string uuid;
    glm::ivec2 screenPos;
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

public:
  const Selection* selection() const
  {
    return m_Selection.has_value() ? &m_Selection.value() : nullptr;
  }

private:
  glm::ivec2 m_MousePos;
  glm::dvec3 m_PointerRay;

  layers::SatelliteLayer* m_SatLayer;
  Globe& m_Globe;
  std::optional<Selection> m_Selection;

  AppContext& m_Ctx;
  SceneState& m_State;

private:
  bool IsVisibleFromCamera(const glm::dvec3& camPos, const glm::dvec3& satPos);
};