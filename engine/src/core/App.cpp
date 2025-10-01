#include <terrakit/core/App.h>
#include <terrakit/core/Scene.h>
#include <terrakit/core/Window.h>
#include <terrakit/core/controllers/CameraControls.h>
#include <terrakit/core/entities/Camera.h>
#include <terrakit/core/entities/Globe.h>
#include <terrakit/core/layers/GlobeLayer.h>
#include <terrakit/platform/MainLoop.h>
#include <terrakit/renderer/Renderer.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>

#include <SDL.h>
#include <terrakit/renderer/OpenGL.h>

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
  m_Window = std::make_unique<Window>(m_Ctx);
  m_RenderContext.SetupDefaultShaders();
  m_Scene = std::make_unique<Scene>(m_Ctx);
}

int
Application::Run()
{
  auto mainLoop = terrakit::platform::MainLoop::Create();
  uint64_t last = SDL_GetPerformanceCounter();

  return mainLoop->Run([this, &last]() -> bool {
    SDL_Event e;

    while (m_Window->PollEvent(e)) {
      ImGui_ImplSDL2_ProcessEvent(&e);
      HandleEvent(e);
    }

    uint64_t now = SDL_GetPerformanceCounter();
    double dt = double(now - last) / SDL_GetPerformanceFrequency();
    last = now;

    m_Scene->Update(dt);
    Update(dt);

    m_RenderContext.BeginFrame(m_Scene->state().camera);
    m_Scene->Render(m_RenderContext);
    Render();

    m_Window->BeginImGuiFrame();
    m_Scene->RenderUI(m_Ctx);
    RenderUI();
    m_Window->EndImGuiFrame();

    m_RenderContext.EndFrame();
    m_Window->Swap();

    return m_Running; // Continue loop while running
  });
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
        m_Ctx.UpdateSize((float)fbW, (float)fbH);
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