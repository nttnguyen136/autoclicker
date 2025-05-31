#pragma once
#include <string>

enum class ActionType
{
    LeftClick,
    RightClick,
    DoubleClick,
    // Add more types as needed
};

struct Action
{
    ActionType type;
    int x;
    int y;
    int delayMs; // delay before action
    int repeat;  // number of times to repeat this action
    std::string comment;

    // Default constructor
    Action()
        : type(ActionType::LeftClick), x(0), y(0), delayMs(0), repeat(1), comment("") {}
};