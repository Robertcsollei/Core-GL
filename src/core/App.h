#pragma once
#include <SDL.h>
#include <core/AppContext.h>
#include <memory>

class Window;
class Renderer;
class Scene;
class CameraControls;

namespace test {
class TestMenu;
}

class Application
{
public:
  Application(int width, int height);
  ~Application();

  virtual void Init();
  int Run();
  const AppContext& Context() const { return m_Ctx; }

protected:
  void HandleEvent(const SDL_Event& e);

  virtual void Update(double dt){};
  virtual void Render(){};
  virtual void RenderUI(){};

protected:
  std::unique_ptr<Renderer> m_Renderer;
  std::unique_ptr<Scene> m_Scene;
  std::unique_ptr<Window> m_Window;

  AppContext m_Ctx;

  bool m_Running = true;
};
