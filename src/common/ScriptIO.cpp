#include "ScriptIO.h"
#include "AppUntils.h"
#include "Action.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace Common
{

    bool SaveScript(const Script &script, const std::string &filename)
    {
        nlohmann::json j;
        Common::to_json(j, script);
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
        nlohmann::json j;
        ifs >> j;
        from_json(j, script);
        return true;
    }

}