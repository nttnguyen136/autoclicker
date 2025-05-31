#pragma once

#include <string>

namespace Common
{

    enum class ActionType
    {
        LeftClick,
        RightClick,
        DoubleClick,
        // Add more types as needed
    };

    struct Action
    {
        Common::ActionType type;
        int x;
        int y;
        int delayMs; // delay before action
        int repeat;  // number of times to repeat this action
        std::string comment;

        // Default constructor
        Action()
            : type(ActionType::LeftClick), x(0), y(0), delayMs(0), repeat(1), comment("") {}
    };

    // Helper to convert ActionType to string
    inline std::string ActionTypeToString(ActionType type)
    {
        switch (type)
        {
        case Common::ActionType::LeftClick:
            return "LeftClick";
        case Common::ActionType::RightClick:
            return "RightClick";
        case Common::ActionType::DoubleClick:
            return "DoubleClick";
        default:
            return "Unknown";
        }
    }

    inline Common::ActionType ActionTypeFromString(const std::string &str)
    {
        if (str == "LeftClick")
            return Common::ActionType::LeftClick;
        if (str == "RightClick")
            return Common::ActionType::RightClick;
        if (str == "DoubleClick")
            return Common::ActionType::DoubleClick;
        return Common::ActionType::LeftClick;
    }
}
