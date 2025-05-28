// src/core/Application.cpp

#include "Application.h"
#include <iostream>

// Include third-party libraries
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

// Constructor: Initialize member variables

Application::Application()
    : m_window(nullptr), m_glContext(nullptr), m_isRunning(false), m_ui(nullptr) {} // Initialize m_ui to nullptr

// Destructor: Call the cleanup function
Application::~Application()
{
    Cleanup();
}

// Main entry point
int Application::Run()
{
    if (!Initialize())
    {
        std::cerr << "Failed to initialize application!" << std::endl;
        return -1;
    }

    m_isRunning = true;
    while (m_isRunning)
    {
        HandleEvents();
        Update();
        Render();
    }

    return 0;
}

// Initialization logic
bool Application::Initialize()
{
    std::cout << "Initializing Application..." << std::endl;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Error: SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_window = SDL_CreateWindow("AutoClicker - Gemini AI", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr)
    {
        SDL_Log("Error: SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1); // Enable V-Sync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_ui = std::make_unique<UI>(m_appState);

    std::cout << "Initialization successful." << std::endl;
    return true;
}

// Event handling logic
void Application::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            m_isRunning = false;
        }
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(m_window))
        {
            m_isRunning = false;
        }
    }
}

// Update logic (for future use)
void Application::Update()
{
    // This is where game logic, physics, etc. would go.
}

// Rendering logic
void Application::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (m_ui) {
        m_ui->Render();
    }

    ImGui::Render();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(m_window);
}

// Cleanup logic
void Application::Cleanup()
{
    std::cout << "Cleaning up..." << std::endl;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if (m_glContext)
    {
        SDL_GL_DestroyContext(m_glContext);
    }
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}