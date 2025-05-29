// src/core/Application.cpp

#include "Application.h"
#include <iostream>
#include "platform/IPlatform.h"

// --- ADD PLATFORM-SPECIFIC INCLUDES ---
#if defined(__APPLE__)
#include "platform/macos/MacOSPlatform.h"
#elif defined(_WIN32)
// #include "platform/windows/WindowsPlatform.h" // For future use
#endif

// Include third-party libraries
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

// Constructor: Initialize member variables

Application::Application()
    : m_window(nullptr), m_glContext(nullptr), m_isRunning(false), m_ui(nullptr), m_platform(nullptr), m_autoclicker(nullptr) {}

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

    m_window = SDL_CreateWindow("AutoClicker - Gemini AI", 800, 600, SDL_WINDOW_OPENGL);
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

    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic(); // Use classic style for better visibility
    ImGui::StyleColorsLight(); // Use light style for better visibility
    // ImGui::StyleColorsDark();    // Use dark style for better visibility

    ImGui_ImplSDL3_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_ui = std::make_unique<UI>(m_appState);
#if defined(__APPLE__)
    m_platform = std::make_unique<MacOSPlatform>();
    std::cout << "Running on macOS, created MacOSPlatform." << std::endl;
#elif defined(_WIN32)
    // m_platform = std::make_unique<WindowsPlatform>(); // For future use
#else
    std::cout << "Running on an unsupported platform." << std::endl;
    // You might want to return false here if the platform is required
#endif

    if (!m_platform)
    {
        std::cerr << "Platform not supported or failed to initialize!" << std::endl;
        return false;
    }

    m_autoclicker = std::make_unique<AutoClicker>(m_appState, *m_platform);

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
    if (m_appState.executionState.load() == ExecutionState::Running)
    {
        m_autoclicker->Start();
    }
    else
    {
        m_autoclicker->Stop();
    }

    // --- LOGIC FOR "PICK COORDINATE" ---
    if (m_appState.isPickingCoordinate)
    {
        HandlePickingMode();
    }
    else
    {
        // If we are NOT in picking mode, and the listener IS active, stop it
        if (m_platform && m_isGlobalListenerActive)
        {
            m_platform->StopGlobalMouseListener();
            m_isGlobalListenerActive = false;
            std::cout << "Application: Stopped global mouse listener." << std::endl;
        }
    }

    if (m_appState.quitRequested)
    {
        m_isRunning = false;
    }
}

// Rendering logic
void Application::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (m_ui)
    {
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

void Application::HandlePickingMode()
{

    // If we are in picking mode, and the listener hasn't been started yet
    if (m_platform && !m_isGlobalListenerActive)
    { // Add m_isGlobalListenerActive to Application.h
        bool success = m_platform->StartGlobalMouseListener(
            // This is a C++ lambda function that will be our callback
            [this](int x, int y)
            {
                // This code will run on the Event Tap thread!
                // Be careful with thread safety if modifying shared AppState directly.
                // For this simple case, we are updating coordinates and a flag.

                std::cout << "Global click detected at: " << x << ", " << y << std::endl;

                // Update AppState (make sure these assignments are safe or post to main thread)
                // For simplicity now, we'll assign directly. Consider a thread-safe queue for complex updates.
                m_appState.currentEditAction.x = x;
                m_appState.currentEditAction.y = y;

                // IMPORTANT: Stop listening immediately after one click and exit picking mode
                m_appState.isPickingCoordinate = false;
                // The StopGlobalMouseListener will be called in the next Update() frame
                // because isPickingCoordinate is now false.
            });
        if (success)
        {
            m_isGlobalListenerActive = true;
            std::cout << "Application: Started global mouse listener for picking." << std::endl;
        }
        else
        {
            std::cout << "Application: Failed to start global mouse listener." << std::endl;
            m_appState.isPickingCoordinate = false; // Reset flag if listener failed
        }
    }
}
