#include "BuiltInFunctionList.h"
#include "BuiltInFunctions.h"
namespace PTL
{
    namespace BuiltIns
    {
        BuiltInFunctionList builtInFunctions;
        BuiltInFunctionList::BuiltInFunctionList(void)
        {
            this->AddFunction("env",        PTLFunc_env);
            this->AddFunction("cat",        PTLFunc_cat);
            this->AddFunction("choose",     PTLFunc_choose);
            this->AddFunction("sum",        PTLFunc_sum);
            this->AddFunction("prod",       PTLFunc_prod);
            this->AddFunction("sub",        PTLFunc_sub);
            this->AddFunction("div",        PTLFunc_div);
            this->AddFunction("fread",      PTLFunc_fread);
            this->AddFunction("round",      PTLFunc_round);
            this->AddFunction("sin",        PTLFunc_sin);
            this->AddFunction("cos",        PTLFunc_cos);
            this->AddFunction("sind",       PTLFunc_sind);
            this->AddFunction("cosd",       PTLFunc_cosd);
            this->AddFunction("max",        PTLFunc_max);
            this->AddFunction("min",        PTLFunc_min);
            this->AddFunction("atan2",      PTLFunc_atan2);
            this->AddFunction("atan2d",     PTLFunc_atan2d);
            this->AddFunction("sqrt",       PTLFunc_sqrt);
            this->AddFunction("lt",         PTLFunc_lt);
            this->AddFunction("gt",         PTLFunc_gt);
            this->AddFunction("pow",        PTLFunc_pow);
        }
        
        bool BuiltInFunctionList::AddFunction(std::string funcName, PTLBuiltinFunction_t func)
        {
            intrinsic.insert({funcName, func});
            return true;
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
    void AddUserDefinedFunction(std::string name, BuiltIns::PTLBuiltinFunction_t func)
    {
        BuiltIns::builtInFunctions.AddFunction(name, func);
    }
}