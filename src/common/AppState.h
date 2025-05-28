// src/common/AppState.h
#pragma once

// This struct holds the shared state of the application that different
// parts of the program might need to read or modify.
struct AppState
{
    // UI-related state
    bool showDemoWindow = true;
    bool showAnotherWindow = false;

    // Autoclicker-related state (for future use)
    bool isClicking = false;
    int clickIntervalMs = 100;

    // A flag to request a test click from the UI
    bool testClickRequested = false;
};