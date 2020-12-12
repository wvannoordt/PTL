#ifndef PTL_BUILTIN_FUNCTIONS_H
#define PTL_BUILTIN_FUNCTIONS_H
#include <string>
#include <vector>
#include "stdlib.h"
namespace PTL
{
    namespace BuiltIns
    {
        std::string PTLFunc_env     (std::vector<std::string>& args);
        std::string PTLFunc_cat     (std::vector<std::string>& args);
        std::string PTLFunc_choose  (std::vector<std::string>& args);
        std::string PTLFunc_sum     (std::vector<std::string>& args);
        std::string PTLFunc_prod    (std::vector<std::string>& args);
        std::string PTLFunc_sub     (std::vector<std::string>& args);
        std::string PTLFunc_div     (std::vector<std::string>& args);
    }
}
#endif