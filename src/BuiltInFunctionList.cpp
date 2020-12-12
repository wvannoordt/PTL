#include "BuiltInFunctionList.h"
#include "BuiltInFunctions.h"
namespace PTL
{
    namespace BuiltIns
    {
        BuiltInFunctionList builtInFunctions;
        BuiltInFunctionList::BuiltInFunctionList(void)
        {
            this->AddFunction("env",    PTLFunc_env);
            this->AddFunction("cat",    PTLFunc_cat);
            this->AddFunction("choose", PTLFunc_choose);
            this->AddFunction("sum",    PTLFunc_sum);
            this->AddFunction("prod",   PTLFunc_prod);
            this->AddFunction("sub",    PTLFunc_sub);
            this->AddFunction("div",    PTLFunc_div);
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