#include "BuiltInFunctions.h"
#include "Error.h"
#include "PropTreeLib.h"

namespace PropTreeLib::BuiltIns
{
    std::string PTLFunc_env(std::vector<std::string>& args)
    {
        std::string p(getenv(args[0].c_str()));
        return p;
    }
    
    std::string PTLFunc_cat(std::vector<std::string>& args)
    {
        std::string output = "";
        for (int i = 0; i < args.size(); i++) output = output + args[i];
        return output;
    }
    
    std::string PTLFunc_choose(std::vector<std::string>& args)
    {
        int idx;
        Variables::PTLInteger i(0, "Choosing index");
        i.ParseFromString(args[0], &idx);
        return args[idx+1];
    }
}