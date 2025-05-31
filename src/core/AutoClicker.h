#pragma once
#include <thread>
#include "common/AppState.h"
#include "platform/IPlatform.h"

class AutoClicker
{
public:
    AutoClicker(AppState &appState, IPlatform &platform);
    ~AutoClicker();

    void Start();
    void Stop();

private:
    void WorkerLoop();

private:
    AppState &m_appState;
    IPlatform &m_platform;
    std::thread m_workerThread;
};