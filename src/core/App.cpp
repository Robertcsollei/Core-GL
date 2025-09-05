#include <core/App.h>

#include <Renderer.h>
#include <core/Scene.h>
#include <core/Window.h>
#include <core/controllers/CameraControls.h>
#include <core/entities/camera/Camera.h>
#include <core/entities/objects/Globe.h>
#include <core/layers/GlobeLayer.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>

#include <SDL.h>
#include <glad/glad.h>

Application::Application(int width, int height)
  : m_Ctx(float(width), float(height))
{
  m_Window = std::make_unique<Window>(m_Ctx.title, width, height);
  int fbW = width, fbH = height;
  m_Window->DrawableSize(fbW, fbH);
  m_Renderer = std::make_unique<Renderer>();
  m_Scene = std::make_unique<Scene>(m_Ctx);
}

Application::~Application() = default;

int
Application::Run()
{
  SDL_Event e;
  uint64_t last = SDL_GetPerformanceCounter();

  while (m_Running) {
    while (m_Window->PollEvent(e)) {
      ImGui_ImplSDL2_ProcessEvent(&e);
      HandleEvent(e);
    }
    uint64_t now = SDL_GetPerformanceCounter();
    double dt = double(now - last) / SDL_GetPerformanceFrequency();
    last = now;

    m_Scene->Update(dt);

    m_Renderer->BeginFrame(m_Scene->state().camera);
    m_Scene->Render(*m_Renderer);

    m_Window->BeginImGuiFrame();
    m_Scene->RenderUI(m_Ctx);
    m_Window->EndImGuiFrame();

    m_Renderer->EndFrame();
    m_Window->Swap();
  }
  return 0;
}

void
Application::HandleEvent(const SDL_Event& e)
{
  switch (e.type) {
    case SDL_QUIT:
      m_Running = false;
      break;
    case SDL_WINDOWEVENT:
      if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        int fbW, fbH;
        m_Window->DrawableSize(fbW, fbH);
        glViewport(0, 0, fbW, fbH);
        m_Scene->OnResize();
      }
      break;
    default:
      break;
  }
  // Always pass input to camera controls
  m_Scene->HandleEvent(e);
}
