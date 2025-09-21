#pragma once
#include <terrakit/core/AppContext.h>

#include <SDL.h>
#include <string>

struct ImGuiContext;
typedef void* SDL_GLContext;
struct SDL_Window;

namespace terrakit::core {

class Window
{
public:
  Window(const AppContext& ctx);
  ~Window();

  // Non-copyable, movable if you need later
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  bool PollEvent(SDL_Event& e);
  void Swap();

  void BeginImGuiFrame();
  void EndImGuiFrame();

  SDL_Window* Sdl() const { return m_Window; }
  void DrawableSize(int& fbW, int& fbH) const;

  void SetVSync(bool enabled);

private:
  void InitSDL();
  void InitGL();
  void InitImGui();

  void ShutdownImGui();
  void ShutdownGL();
  void ShutdownSDL();

private:
  SDL_Window* m_Window = nullptr;
  SDL_GLContext m_GLContext = nullptr;
  ImGuiContext* m_ImGuiCtx = nullptr;

  const AppContext& m_Ctx;
};
} // namespace terrakit::core