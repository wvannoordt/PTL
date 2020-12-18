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
        std::string PTLFunc_fread   (std::vector<std::string>& args);
        std::string PTLFunc_round   (std::vector<std::string>& args);
        std::string PTLFunc_sin     (std::vector<std::string>& args);
        std::string PTLFunc_cos     (std::vector<std::string>& args);
        std::string PTLFunc_sind    (std::vector<std::string>& args);
        std::string PTLFunc_cosd    (std::vector<std::string>& args);
        std::string PTLFunc_max     (std::vector<std::string>& args);
        std::string PTLFunc_min     (std::vector<std::string>& args);
        std::string PTLFunc_atan2   (std::vector<std::string>& args);
        std::string PTLFunc_atan2d  (std::vector<std::string>& args);
        std::string PTLFunc_sqrt    (std::vector<std::string>& args);
    }
}
#endif