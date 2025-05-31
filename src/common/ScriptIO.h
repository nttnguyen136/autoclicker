#pragma once
#include "Script.h"
#include <string>

namespace ScriptIO
{
    // Save script to JSON file. Return true if success.
    bool SaveToFile(const Script &script, const std::string &filename);

    // Load script from JSON file. Return true if success.
    bool LoadFromFile(Script &script, const std::string &filename);
}