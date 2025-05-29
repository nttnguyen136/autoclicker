// src/common/AppState.h
#pragma once

#include <vector>
#include <string>
#include <mutex>  // Required for std::mutex
#include <atomic> // Required for std::atomic

// Enum to represent the execution state of the script
enum class ExecutionState
{
    Stopped,
    Running,
    Paused
};

// Enum for mouse action types
enum class ActionType
{
    LeftClick,
    RightClick,
    DoubleClick
};

// Struct for a single action
struct Action
{
    ActionType type = ActionType::LeftClick;
    int x = 0;
    int y = 0;
    int delayMs = 100;
    std::string comment;
};

// The new, simplified, thread-safe AppState
struct AppState
{
    // --- Data Model ---
    // The list of actions. This will be accessed by both threads.
    std::vector<Action> actions;

    // The mutex to protect the 'actions' vector from data races.
    // Any thread that wants to read or write to 'actions' MUST lock this mutex first.
    std::mutex actionsMutex;

    // --- High-Level Control State ---
    // The current state of the script execution. std::atomic makes it thread-safe for simple reads/writes.
    std::atomic<ExecutionState> executionState = ExecutionState::Stopped;

    // --- UI-related state ---
    // These are only accessed by the UI thread, so they don't need protection.
    int selectedActionIndex = -1;
    Action currentEditAction; // For the Add/Edit panel
    bool quitRequested = false;
};