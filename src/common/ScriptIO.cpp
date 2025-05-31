#include "ScriptIO.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Common
{

    // Helper to convert ActionType to string
    std::string ActionTypeToString(ActionType type)
    {
        switch (type)
        {
        case ActionType::LeftClick:
            return "LeftClick";
        case ActionType::RightClick:
            return "RightClick";
        case ActionType::DoubleClick:
            return "DoubleClick";
        default:
            return "Unknown";
        }
    }

    ActionType ActionTypeFromString(const std::string &str)
    {
        if (str == "LeftClick")
            return ActionType::LeftClick;
        if (str == "RightClick")
            return ActionType::RightClick;
        if (str == "DoubleClick")
            return ActionType::DoubleClick;
        return ActionType::LeftClick;
    }

    void to_json(json &j, const Action &a)
    {
        j = json{
            {"type", ActionTypeToString(a.type)},
            {"x", a.x},
            {"y", a.y},
            {"delayMs", a.delayMs},
            {"repeat", a.repeat},
            {"comment", a.comment}};
    }

    void from_json(const json &j, Action &a)
    {
        a.type = ActionTypeFromString(j.value("type", "LeftClick"));
        a.x = j.value("x", 0);
        a.y = j.value("y", 0);
        a.delayMs = j.value("delayMs", 0);
        a.repeat = j.value("repeat", 1);
        a.comment = j.value("comment", "");
    }

    void to_json(json &j, const Script &s)
    {
        j["name"] = s.name;
        j["actions"] = json::array();
        for (const auto &action : s.actions)
        {
            json action_json;
            to_json(action_json, action);
            j["actions"].push_back(action_json);
        }
    }

    void from_json(const json &j, Script &s)
    {
        s.name = j.value("name", "Unnamed Script");
        if (j.contains("actions"))
        {
            s.actions.clear();
            for (const auto &item : j.at("actions"))
            {
                Action a;
                from_json(item, a);
                s.actions.push_back(a);
            }
        }
        else
            s.actions = {};
    }

} // anonymous namespace

namespace Common
{

    bool SaveScript(const Script &script, const std::string &filename)
    {
        json j;
        to_json(j, script);
        std::ofstream ofs(filename);
        if (!ofs)
            return false;
        ofs << j.dump(4);
        return true;
    }

    bool LoadScript(Script &script, const std::string &filename)
    {
        std::ifstream ifs(filename);
        if (!ifs)
            return false;
        json j;
        ifs >> j;
        from_json(j, script);
        return true;
    }

} // namespace ScriptIO