// src/ui/UI.h
#pragma once

// Forward declare AppState to avoid including the header here
struct AppState;

class UI {
public:
    // Constructor takes a reference to the shared application state
    UI(AppState& appState);

    // The main function to draw all UI elements
    void Render();

private:
    // A reference to the application's shared state
    AppState& m_appState;
};