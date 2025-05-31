#pragma once
#include <string>
#include <vector>
#include "Action.h"

struct Script
{
    std::string name;
    std::vector<Common::Action> actions;

    Script() : name("Unnamed Script") {}
};