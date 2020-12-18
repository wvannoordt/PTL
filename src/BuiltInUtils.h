#ifndef PTL_BUILTIN_UTILS_H
#define PTL_BUILTIN_UTILS_H
#include <string>
#include <vector>
#include "stdlib.h"
#include "BuiltInFunctionList.h"
namespace PTL
{
    namespace BuiltIns
    {
        double AssertConvertDouble(std::string a);
        int AssertConvertInt(std::string a);
        inline bool FileExists(const std::string& name);
        std::vector<std::string> ReadFileToStringVector(std::string filename);
    }
}

#endif