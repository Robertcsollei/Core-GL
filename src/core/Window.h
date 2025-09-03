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

  bool pollEvent(SDL_Event& e);
  void swap();

  void beginImGuiFrame();
  void endImGuiFrame();

  SDL_Window* sdl() const { return m_Window; }
  void drawableSize(int& fbW, int& fbH) const;

  void setVSync(bool enabled);

private:
  void initSDL(const std::string& title, int width, int height);
  void initGL();
  void initImGui();

  void shutdownImGui();
  void shutdownGL();
  void shutdownSDL();

private:
  SDL_Window* m_Window = nullptr;
  SDL_GLContext m_GLContext = nullptr;
  ImGuiContext* m_ImGuiCtx = nullptr;
};
