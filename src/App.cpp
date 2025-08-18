#include <iostream>

// Include GLAD before any OpenGL headers
#include <glad/glad.h>
#include <SDL.h>

#include <fstream>
#include <sstream>
#include <string>
#include "data/basic.shader.h"
#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>






int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);

    // Create an OpenGL window
    SDL_Window* win = SDL_CreateWindow(
        "SDL2 + GLAD (triangle)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
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

    // Optional: enable vsync
    SDL_GL_SetSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(win, ctx);
    ImGui_ImplOpenGL3_Init("#version 330 core");


    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    // Basic state

    int winW, winH, fbW, fbH;
    SDL_GetWindowSize(win, &winW, &winH);
    SDL_GL_GetDrawableSize(win, &fbW, &fbH);
    glViewport(0, 0, 800, 600);
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);


    {


        float positions[] = {
             -50.f, -50.f, 0.0f, 0.0f, // 0
              50.f, -50.f, 1.0f, 0.0f, // 1
              50.f,  50.f, 1.0f, 1.0f, // 2
             -50.f,  50.f, 0.0f, 1.0f  // 3
        };


        unsigned int indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        unsigned int vao = 0;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2); // 2 floats per vertex (x, y)
        layout.Push<float>(2); // 2 floats for texture coordinates (u, v)

        va.addVertexBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        std::string vertexShaderSource = Shaders::vertex;
        std::string fragmentShaderSource = Shaders::fragment;


        glm::mat4 proj = glm::ortho(0.0f, float(winW), 0.0f, float(winH), -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader(vertexShaderSource, fragmentShaderSource);
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


        Renderer renderer;

        unsigned int textureSlot = 0;
        Texture texture("res/textures/test.png");
        texture.Bind(textureSlot);
        shader.SetUniform1i("u_Texture", textureSlot);

        bool running = true;
        SDL_Event e;


        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        while (running) {
            while (SDL_PollEvent(&e)) {
                ImGui_ImplSDL2_ProcessEvent(&e);
                if (e.type == SDL_QUIT) running = false;
                if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int w = e.window.data1;
                    int h = e.window.data2;

                    int fbW, fbH;
                    SDL_GL_GetDrawableSize(win, &fbW, &fbH);
                    GLCall(glViewport(0, 0, fbW, fbH));
                    proj = glm::ortho(0.0f, float(w), 0.0f, float(h), -1.0f, 1.0f);
                }
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

           
            renderer.Clear();

            shader.Bind();  
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model; 
                shader.SetUniformMat4f("u_MVP", mvp); 
                renderer.Draw(va, ib, shader); 
            }


            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 640.f);
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 640.f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(win);
        }
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
