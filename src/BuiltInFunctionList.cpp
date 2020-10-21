#include "BuiltInFunctionList.h"
#include "BuiltInFunctions.h"
namespace PropTreeLib
{
    namespace BuiltIns
    {
        BuiltInFunctionList builtInFunctions;
        BuiltInFunctionList::BuiltInFunctionList(void)
        {
            this->AddFunction("env",    PTLFunc_env);
            this->AddFunction("cat",    PTLFunc_cat);
            this->AddFunction("choose", PTLFunc_choose);
            
        }
        
        bool BuiltInFunctionList::AddFunction(std::string funcName, PTLBuiltinFunction_t func)
        {
            intrinsic.insert({funcName, func});
        }
        
        bool BuiltInFunctionList::Exists(std::string funcName)
        {
            return (intrinsic.find(funcName)!=intrinsic.end());
        }
        
        PTLBuiltinFunction_t& BuiltInFunctionList::operator [](std::string funcName)
        {
            return intrinsic[funcName];
        }
        
        BuiltInFunctionList::~BuiltInFunctionList(void)
        {
            
        }
    }
}