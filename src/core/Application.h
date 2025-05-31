#pragma once
#include <SDL3/SDL.h>
#include <memory>

#include "common/AppState.h"
#include "common/Action.h"
#include "common/Script.h"
#include "core/AutoClicker.h"
#include "ui/UI.h"

class IPlatform;

class Application
{
public:
    Application();
    ~Application();

    int Run();

private:
    bool Initialize();
    void HandleEvents();
    void Update();
    void Render();
    void Cleanup();

    void HandlePickingMode();

    void StartRecording();
    void StopRecording(const std::string &name);
    bool ImportScript(const std::string &filename);
    bool ExportScript(const std::string &filename);

    Script &GetCurrentScript() { return m_currentScript; }
    void SetCurrentScript(const Script &script) { m_currentScript = script; }

private:
    SDL_Window *m_window;
    SDL_GLContext m_glContext;
    bool m_isRunning;
    bool m_isGlobalListenerActive = false;

    AppState m_appState;                        // Holds the shared state
    std::unique_ptr<UI> m_ui;                   // Pointer to our UI manager class
    std::unique_ptr<IPlatform> m_platform;      // Pointer to our platform implementation
    std::unique_ptr<AutoClicker> m_autoclicker; // Pointer to the AutoClicker class that handles the clicking logic

    Script m_currentScript;
    bool m_isRecording = false;
};