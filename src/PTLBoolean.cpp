#include <iostream>
#include <string>
#include "PTLBoolean.h"
namespace PTL
{
    PTLBoolean::PTLBoolean(bool defaultValueIn, std::string descriptionIn)
    {
        this->SetDescription(descriptionIn);
        defaultValue = defaultValueIn;
        basePointerType = BasePointer::BoolPointer;
    }
    bool PTLBoolean::ParseFromString(std::string parseVal, void* ptr)
    {
        if (hasBeenParsed) return true;
        if ((parseVal=="True")||(parseVal=="true")||(parseVal=="T")||(parseVal=="t"))
        {
            *((bool*)ptr) = true;
            hasBeenParsed = true;
            return true;
        }
        if ((parseVal=="False")||(parseVal=="false")||(parseVal=="F")||(parseVal=="f"))
        {
            *((bool*)ptr) = false;
            hasBeenParsed = true;
            return true;
        }
        parseErrorString = "could not parse entry to bool.";
        this->SetDefaultValue(ptr);
        return false;
    }
    std::string PTLBoolean::GetDefaultValueString(void)
    {
        return std::to_string(defaultValue);
    }
    void PTLBoolean::SetDefaultValue(void* ptr)
    {
        *((bool*)ptr) = defaultValue;
    }
    void PTLBoolean::Destroy(void){}
}
