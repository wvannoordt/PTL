#ifndef PTL_BUILTINS_H
#define PTL_BUILTINS_H
#include <map>
#include <string>
#include <vector>
namespace PropTreeLib
{
    namespace BuiltIns
    {
        typedef std::string(*PTLBuiltinFunction_t)(std::vector<std::string>&);
        class BuiltInFunctionList
        {
            public:
                BuiltInFunctionList(void);
                ~BuiltInFunctionList(void);
                bool AddFunction(std::string funcName, PTLBuiltinFunction_t func);
                bool Exists(std::string funcName);
                PTLBuiltinFunction_t& operator [](std::string funcName);
            private:
                std::map<std::string, PTLBuiltinFunction_t> intrinsic;
        };
        extern BuiltInFunctionList builtInFunctions;
    }
}

#endif