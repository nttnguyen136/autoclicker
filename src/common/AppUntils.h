#pragma once

#include <vector>
#include "AppState.h"
#include "Action.h"

std::vector<Action> GetActionsCopy(AppState &appState)
{
    std::lock_guard<std::mutex> lock(appState.actionsMutex);
    return appState.actions; // Return a copy of the actions vector
}