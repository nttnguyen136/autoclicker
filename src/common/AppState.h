// src/common/AppState.h
#pragma once

#include <vector>
#include <string>

// Enum to represent the type of mouse action
enum class ActionType
{
    LeftClick,
    RightClick,
    DoubleClick,
    ScrollUp,
    ScrollDown
    // ... add more types as needed
};

// A struct to hold all data for a single action in our list
struct Action
{
    ActionType type = ActionType::LeftClick;
    int x = 0;
    int y = 0;
    bool cursorBack = false;
    int delayMs = 100;
    int repeatCount = 1;
    std::string comment;
};

// This struct holds the entire state of our application
struct AppState
{
    // The list of actions to be executed
    std::vector<Action> actions;

    // The index of the currently selected action in the list (-1 means none selected)
    int selectedActionIndex = -1;

    // A temporary Action object used for the "Add/Edit" panel
    Action currentEditAction;

    // Global execution state
    bool isExecutingScript = false;
    int executionRepeatCount = 1;

    // State flags for UI interaction
    bool isPickingCoordinate = false;
    bool quitRequested = false;

    // Hotkey configurations (as strings for now)
    std::string hotkeyAddPosition = "F6";
    std::string hotkeyGetPosition = "None";
    std::string hotkeyStartStop = "None";

    // Real-time data for the status bar
    int currentMouseX = 0;
    int currentMouseY = 0;
};