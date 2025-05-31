#pragma once

#include <vector>
#include "AppState.h"
#include "Action.h"
#include "Script.h"
#include <nlohmann/json.hpp>

namespace Common
{
    inline std::vector<Common::Action> GetActionsCopy(AppState &appState)
    {
        std::lock_guard<std::mutex> lock(appState.actionsMutex);
        return appState.actions; // Return a copy of the actions vector
    }

    inline void to_json(nlohmann::json &j, const Common::Action &a)
    {
        j = nlohmann::json{
            {"type", ActionTypeToString(a.type)},
            {"x", a.x},
            {"y", a.y},
            {"delayMs", a.delayMs},
            {"repeat", a.repeat},
            {"comment", a.comment}};
    }

    inline void from_json(const nlohmann::json &j, Common::Action &a)
    {
        a.type = ActionTypeFromString(j.value("type", "LeftClick"));
        a.x = j.value("x", 0);
        a.y = j.value("y", 0);
        a.delayMs = j.value("delayMs", 0);
        a.repeat = j.value("repeat", 1);
        a.comment = j.value("comment", "");
    }

    inline void to_json(nlohmann::json &j, const Script &s)
    {
        j["name"] = s.name;
        j["actions"] = nlohmann::json::array();
        for (const auto &action : s.actions)
        {
            nlohmann::json action_json;
            Common::to_json(action_json, action);
            j["actions"].push_back(action_json);
        }
    }

    inline void from_json(const nlohmann::json &j, Script &s)
    {
        s.name = j.value("name", "Unnamed Script");
        if (j.contains("actions"))
        {
            s.actions.clear();
            for (const auto &item : j.at("actions"))
            {
                Common::Action a;
                from_json(item, a);
                s.actions.push_back(a);
            }
        }
        else
            s.actions = {};
    }
}