// src/common/AppState.h
#pragma once

#include <vector>
#include <string>
#include <mutex>  // Required for std::mutex
#include <atomic> // Required for std::atomic

#include "Action.h"

// Enum to represent the execution state of the script
enum class ExecutionState
{
    Stopped,
    Running,
    Paused
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

    bool isPickingCoordinate = false;

    // Hotkey configurations (as strings for now)
    std::string hotkeyAddPosition = "F6";
    std::string hotkeyGetPosition = "None";
    std::string hotkeyStartStop = "None";

    // Real-time data for the status bar
    int currentMouseX = 0;
    int currentMouseY = 0;
};