#include <terrakit/core/App.h>
#include <terrakit/core/Scene.h>
#include <terrakit/core/Window.h>
#include <terrakit/core/controllers/CameraControls.h>
#include <terrakit/core/entities/Camera.h>
#include <terrakit/core/entities/Globe.h>
#include <terrakit/core/layers/GlobeLayer.h>
#include <terrakit/renderer/Renderer.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>

#include <SDL.h>
#include <glad/glad.h>

using namespace terrakit::renderer;

namespace terrakit::core {

Application::Application(int width, int height)
  : m_Ctx(float(width), float(height))
{
}

Application::~Application() = default;

void
Application::Init()
{
  int fbW = int(m_Ctx.width);
  int fbH = int(m_Ctx.height);

  m_Window = std::make_unique<Window>(m_Ctx.title, fbW, fbH);
  m_Window->DrawableSize(fbW, fbH);
  m_Renderer = std::make_unique<Renderer>();
  m_Scene = std::make_unique<Scene>(m_Ctx);
}

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
    Update(dt);

    m_Renderer->BeginFrame(m_Scene->state().camera);
    m_Scene->Render(*m_Renderer);
    Render();

    m_Window->BeginImGuiFrame();
    m_Scene->RenderUI(m_Ctx);
    RenderUI();
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
        m_Ctx.width = (float)fbW;
        m_Ctx.height = (float)fbH;
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

}