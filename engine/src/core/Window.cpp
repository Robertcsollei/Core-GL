#include <terrakit/core/Window.h>

#include <iostream>
#include <stdexcept>

#include <glad/glad.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>

namespace {
inline void
sdlCheck(bool ok, const char* what)
{
  if (!ok)
    throw std::runtime_error(std::string(what) + ": " + SDL_GetError());
}
}
namespace terrakit::core {
Window::Window(const AppContext& ctx)
  : m_Ctx(ctx)
{
  InitSDL();
  InitGL();
  InitImGui();
}

Window::~Window()
{
  ShutdownImGui();
  ShutdownGL();
  ShutdownSDL();
}

void
Window::InitSDL()
{
  const int width = int(m_Ctx.width);
  const int height = int(m_Ctx.height);

  sdlCheck(SDL_Init(SDL_INIT_VIDEO) == 0, "SDL_Init");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  m_Window = SDL_CreateWindow(m_Ctx.title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width,
                              height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
                                SDL_WINDOW_RESIZABLE);
  sdlCheck(m_Window != nullptr, "SDL_CreateWindow");

  m_GLContext = SDL_GL_CreateContext(m_Window);
  sdlCheck(m_GLContext != nullptr, "SDL_GL_CreateContext");

  SDL_SetWindowResizable(m_Window, SDL_TRUE);

  std::string iconPath = m_Ctx.iconPath + "favicon.ico";
  SDL_Surface* icon = SDL_LoadBMP(iconPath.c_str());
  if (icon) {
    SDL_SetWindowIcon(m_Window, icon);
    SDL_FreeSurface(icon);
  } else {
    std::cout
      << "Warning: Could not load window icon from assets/images/favicon.ico"
      << std::endl;
  }
}

void
Window::InitGL()
{
  // Load GL functions via GLAD using SDL loader
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }
  SetVSync(true);

  int fbW, fbH;
  DrawableSize(fbW, fbH);
  glViewport(0, 0, fbW, fbH);

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

void
Window::InitImGui()
{
  IMGUI_CHECKVERSION();
  m_ImGuiCtx = ImGui::CreateContext();

  // Optional: DPI/content scale
  float scale = ImGui_ImplSDL2_GetContentScaleForWindow(m_Window);
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(scale);
  style.FontScaleDpi = scale;

  ImVec4* colors = style.Colors;
  colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(1.00f, 0.55f, 0.00f, 0.80f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.65f, 0.10f, 0.90f);
  colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.55f, 0.00f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(1.00f, 0.55f, 0.00f, 0.70f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.65f, 0.10f, 0.80f);
  colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.55f, 0.00f, 1.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.55f, 0.00f, 0.40f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.55f, 0.00f, 0.70f);
  colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.55f, 0.00f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.55f, 0.00f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.65f, 0.10f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.25f, 0.00f, 0.70f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.55f, 0.00f, 0.30f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.65f, 0.10f, 0.70f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.55f, 0.00f, 1.00f);

  ImGui_ImplSDL2_InitForOpenGL(m_Window, m_GLContext);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void
Window::ShutdownImGui()
{
  if (m_ImGuiCtx) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(m_ImGuiCtx);
    m_ImGuiCtx = nullptr;
  }
}

void
Window::ShutdownGL()
{
  if (m_GLContext) {
    SDL_GL_DeleteContext(m_GLContext);
    m_GLContext = nullptr;
  }
}

void
Window::ShutdownSDL()
{
  if (m_Window) {
    SDL_DestroyWindow(m_Window);
    m_Window = nullptr;
  }
  SDL_Quit();
}

bool
Window::PollEvent(SDL_Event& e)
{
  return SDL_PollEvent(&e);
}

void
Window::Swap()
{
  SDL_GL_SwapWindow(m_Window);
}

void
Window::BeginImGuiFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void
Window::EndImGuiFrame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
Window::DrawableSize(int& fbW, int& fbH) const
{
  SDL_GL_GetDrawableSize(m_Window, &fbW, &fbH);
}

void
Window::SetVSync(bool enabled)
{
  SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}
} // namespace terrakit::core