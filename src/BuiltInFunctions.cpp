#include "BuiltInFunctions.h"
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
}