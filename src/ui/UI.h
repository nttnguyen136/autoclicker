#pragma once

struct AppState;

class UI
{
public:
    UI(AppState &appState);
    void Render();

private:
    AppState &m_appState;

    // Draws the top menu bar with file operations.
    void RenderMenuBar();

    // Renders the main content view of the application.
    void RenderMainView();

    // Displays the status bar with application information.
    void RenderStatusBar();
};