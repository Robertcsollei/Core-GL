#pragma once

#include <SDL.h>
#include <string>

struct ImGuiContext;
typedef void* SDL_GLContext;
struct SDL_Window;

class Window
{
public:
  Window(const std::string& title, int width, int height);
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
  void InitSDL(const std::string& title, int width, int height);
  void InitGL();
  void InitImGui();

  void ShutdownImGui();
  void ShutdownGL();
  void ShutdownSDL();

private:
  SDL_Window* m_Window = nullptr;
  SDL_GLContext m_GLContext = nullptr;
  ImGuiContext* m_ImGuiCtx = nullptr;
};
