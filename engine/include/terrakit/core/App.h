#pragma once
#include <SDL.h>
#include <memory>
#include <terrakit/core/AppContext.h>

namespace terrakit::renderer {
class Renderer;
}

namespace terrakit::core {

class Window;
class Scene;
class CameraControls;

class Application
{
public:
  Application(int width, int height);
  ~Application();

  virtual void Init();
  int Run();
  const AppContext& Context() const { return m_Ctx; }

#ifdef __EMSCRIPTEN__
  void MainLoopIteration();
#endif

protected:
  void HandleEvent(const SDL_Event& e);

  virtual void Update(double dt){};
  virtual void Render(){};
  virtual void RenderUI(){};

protected:
  std::unique_ptr<terrakit::renderer::Renderer> m_Renderer;
  std::unique_ptr<Scene> m_Scene;
  std::unique_ptr<Window> m_Window;

  AppContext m_Ctx;

  bool m_Running = true;
};
}