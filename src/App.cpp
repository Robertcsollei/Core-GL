#include <iostream>

// Include GLAD before any OpenGL headers
#include <SDL.h>
#include <glad/glad.h>

#include "Renderer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>

#include "./entities/Camera.h"
#include "entities/CameraControls.h"

#include "tests/TestMenu.h"

int
main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);

  SDL_Window* win = SDL_CreateWindow("OpenGl Renderer",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     1240,
                                     860,
                                     SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (!win) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Create GL context
  SDL_GLContext ctx = SDL_GL_CreateContext(win);
  if (!ctx) {
    std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  SDL_SetWindowResizable(win, SDL_TRUE);

  // Load GL functions via GLAD using SDL's loader
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  SDL_GL_SetSwapInterval(1);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup scaling
  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);
  style.FontScaleDpi = main_scale;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(win, ctx);
  ImGui_ImplOpenGL3_Init("#version 130");
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  int winW, winH, fbW, fbH;
  SDL_GetWindowSize(win, &winW, &winH);
  SDL_GL_GetDrawableSize(win, &fbW, &fbH);
  glViewport(0, 0, 1240, 860);
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);

  unsigned int vao = 0;
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  Renderer renderer;
  renderer.SetWireframeMode(false);

  bool running = true;
  SDL_Event e;

  Camera camera(45.f, 1240.f, 860.f, 0.1f, 10000.f);
  camera.setLookAt({ 0, 0, 300 }, { 0.f, 0.f, -800.f }, { 0, 1, 0 });
  CameraControls camCtl(&camera);

  test::TestMenu testMenu(&renderer, camera);

  while (running) {
    while (SDL_PollEvent(&e)) {
      ImGui_ImplSDL2_ProcessEvent(&e);
      if (e.type == SDL_QUIT)
        running = false;
      if (e.type == SDL_WINDOWEVENT &&
          e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        int w = e.window.data1;
        int h = e.window.data2;

        int fbW, fbH;
        SDL_GL_GetDrawableSize(win, &fbW, &fbH);
        GLCall(glViewport(0, 0, fbW, fbH));
        camera.onResize((float)fbW, (float)fbH);
      }
      camCtl.Update(e);
    }

    renderer.BeginFrame(camera);

    testMenu.OnUpdate(0.0f);
    testMenu.OnRender();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    testMenu.OnImGuiRender();
    ImGui::Render();

    renderer.EndFrame();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(win);
  }

  // ImGui Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
