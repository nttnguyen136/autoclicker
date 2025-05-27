#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

int main(int argc, char* argv[]) {

    std::cout << "Application starting..." << std::endl;
    // 1. Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {// Add this line
        std::cerr << "Fatal Error: SDL_Init failed: " << SDL_GetError() << std::endl;
        SDL_Log("Error: %s\n", SDL_GetError());
        return -1;
    }

    std::cout << "SDL initialized successfully." << std::endl;

    // Setup OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 2. Create SDL window
    SDL_Window* window = SDL_CreateWindow(
        "AutoClicker - Gemini AI",
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == nullptr) {
        SDL_Log("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // 3. Create OpenGL context for the window
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable V-Sync

    // 4. Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable window docking
    
    ImGui::StyleColorsDark(); // Choose dark color theme

    // Setup ImGui for SDL3 and OpenGL3
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    // 5. Main application loop
    bool done = false;
    while (!done) {
        // Handle events (mouse, keyboard input)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)) {
                done = true;
            }
        }

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Draw your ImGui interface here
        ImGui::ShowDemoWindow(); // Show Demo window for testing

        // Simple "Hello World" window
        ImGui::Begin("Hello from Gemini AI!");
        ImGui::Text("This is the first step to build an AutoClicker.");
        ImGui::End();

        // Render ImGui frame
        ImGui::Render();
        
        // Clear screen with background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw ImGui data to screen
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Swap buffers to display new frame
        SDL_GL_SwapWindow(window);
    }

    // 6. Cleanup before exit
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}