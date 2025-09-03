#include "core/App.h"

#include "Renderer.h"
#include "core/Window.h"
#include "core/controllers/CameraControls.h"
#include "core/entities/camera/Camera.h"
#include "tests/TestMenu.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>

#include <SDL.h>
#include <glad/glad.h>

Application::Application(int width, int height)
{
  m_Window = std::make_unique<Window>("OpenGL Renderer", width, height);
  m_Renderer = std::make_unique<Renderer>();

  // You can query drawable size right after Window init
  int fbW = width, fbH = height;
  m_Window->drawableSize(fbW, fbH);

  m_Camera =
    std::make_unique<Camera>(45.f, (float)fbW, (float)fbH, 0.1f, 10000.f);
  m_Camera->setLookAt({ 0, 0, 300 }, { 0, 0, -800 }, { 0, 1, 0 });
  m_CamCtl = std::make_unique<CameraControls>(m_Camera.get());

  m_TestMenu = std::make_unique<test::TestMenu>(m_Renderer.get(), *m_Camera);

  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
}

Application::~Application() = default;

int
Application::run()
{
  SDL_Event e;
  while (m_Running) {
    while (m_Window->pollEvent(e)) {
      ImGui_ImplSDL2_ProcessEvent(&e);
      handleEvent(e);
    }

    m_Renderer->BeginFrame(*m_Camera);

    // Update & render your scene/tests
    m_TestMenu->OnUpdate(0.0f);
    m_TestMenu->OnRender();

    // ImGui
    m_Window->beginImGuiFrame();
    m_TestMenu->OnImGuiRender();
    m_Window->endImGuiFrame();

    m_Renderer->EndFrame();

    m_Window->swap();
  }
  return 0;
}

void
Application::handleEvent(const SDL_Event& e)
{
  switch (e.type) {
    case SDL_QUIT:
      m_Running = false;
      break;
    case SDL_WINDOWEVENT:
      if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        int fbW, fbH;
        m_Window->drawableSize(fbW, fbH);
        glViewport(0, 0, fbW, fbH);
        m_Camera->onResize((float)fbW, (float)fbH);
      }
      break;
    default:
      break;
  }
  // Always pass input to camera controls
  m_CamCtl->Update(e);
}
