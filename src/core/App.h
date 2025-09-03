#pragma once
#include <SDL.h>
#include <memory>

class Window;
class Renderer;
class Camera;
class CameraControls;

namespace test {
class TestMenu;
}

class Application
{
public:
  Application(int width, int height);
  ~Application();

  int run();

private:
  void handleEvent(const SDL_Event& e);

private:
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<Renderer> m_Renderer;
  std::unique_ptr<Camera> m_Camera;
  std::unique_ptr<CameraControls> m_CamCtl;
  std::unique_ptr<test::TestMenu> m_TestMenu;

  bool m_Running = true;
};
