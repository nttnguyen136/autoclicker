// src/core/AutoClicker.h
#pragma once
#include <thread>
#include "common/AppState.h"
#include "platform/IPlatform.h"

class AutoClicker
{
public:
    // Constructor takes references to the shared state and platform interface
    AutoClicker(AppState &appState, IPlatform &platform);
    ~AutoClicker();

    // Public methods to control the worker thread
    void Start();
    void Stop();

private:
    // The main function that the worker thread will execute
    void WorkerLoop();

private:
    AppState &m_appState;
    IPlatform &m_platform;
    std::thread m_workerThread;
};