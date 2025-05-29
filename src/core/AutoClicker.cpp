// src/core/AutoClicker.cpp
#include "AutoClicker.h"
#include <iostream>
#include <vector>

AutoClicker::AutoClicker(AppState &appState, IPlatform &platform)
    : m_appState(appState), m_platform(platform) {}

AutoClicker::~AutoClicker()
{
    Stop(); // Ensure the thread is stopped when the object is destroyed
}

void AutoClicker::Start()
{
    // If the state is not already running, start the thread
    if (m_appState.executionState.load() != ExecutionState::Running)
    {
        m_appState.executionState.store(ExecutionState::Running);

        // Stop any previous thread before starting a new one
        Stop();

        // Create and launch the new worker thread, telling it to run the WorkerLoop function
        m_workerThread = std::thread(&AutoClicker::WorkerLoop, this);
        std::cout << "Worker thread started." << std::endl;
    }
}

void AutoClicker::Stop()
{
    // Set the state to Stopped to signal the thread to exit its loop
    m_appState.executionState.store(ExecutionState::Stopped);

    // If the thread is joinable (i.e., running), wait for it to finish
    if (m_workerThread.joinable())
    {
        m_workerThread.join();
        std::cout << "Worker thread stopped." << std::endl;
    }
}

void AutoClicker::WorkerLoop()
{
    while (m_appState.executionState.load() == ExecutionState::Running)
    {
        // --- Thread-Safe Data Copy ---
        std::vector<Action> actions_copy;
        {
            // Lock the mutex to safely access the shared actions vector
            std::lock_guard<std::mutex> lock(m_appState.actionsMutex);

            // Make a copy of the actions. This is a critical pattern!
            // We do this so we can release the lock quickly and not block the UI thread
            // while we are performing the long-running actions (with delays).
            actions_copy = m_appState.actions;
        } // The lock is automatically released here

        if (actions_copy.empty())
        {
            std::cout << "Action list is empty, stopping." << std::endl;
            break; // Exit the loop if there's nothing to do
        }

        // Now, iterate over the *copy* of the actions
        for (const auto &action : actions_copy)
        {
            // Check if a stop was requested during the loop
            if (m_appState.executionState.load() != ExecutionState::Running)
            {
                std::cout << "Execution stopped by user." << std::endl;
                return; // Exit the function and the thread
            }

            // Perform the delay
            std::this_thread::sleep_for(std::chrono::milliseconds(action.delayMs));

            // Perform the click via the platform interface
            m_platform.SimulateMouseClick();
        }
    }
    // If the loop finishes naturally, set the state back to Stopped
    m_appState.executionState.store(ExecutionState::Stopped);
    std::cout << "Worker loop finished." << std::endl;
}