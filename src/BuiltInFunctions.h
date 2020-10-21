#ifndef PTL_BUILTIN_FUNCTIONS_H
#define PTL_BUILTIN_FUNCTIONS_H
#include <string>
#include <vector>
#include "stdlib.h"
namespace PropTreeLib::BuiltIns
{
    std::string PTLFunc_env    (std::vector<std::string>& args);
    std::string PTLFunc_cat    (std::vector<std::string>& args);
    std::string PTLFunc_choose (std::vector<std::string>& args);
}

#endif