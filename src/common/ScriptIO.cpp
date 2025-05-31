#include "ScriptIO.h"
#include "AppUntils.h"
#include "Action.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Common
{

    bool SaveScript(const Script &script, const std::string &filename)
    {
        json j;
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
        json j;
        ifs >> j;
        from_json(j, script);
        return true;
    }

}