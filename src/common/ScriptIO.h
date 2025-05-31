#pragma once
#include "Script.h"
#include <string>

namespace Common
{
    // Save script to JSON file. Return true if success.
    bool SaveScript(const Script &script, const std::string &filename);

    // Load script from JSON file. Return true if success.
    bool LoadScript(Script &script, const std::string &filename);
}