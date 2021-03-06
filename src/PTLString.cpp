#include <iostream>
#include <string>
#include "PTLString.h"

namespace PTL
{
    PTLString::PTLString(std::string defaultValueIn, std::string descriptionIn)
    {
        this->SetDescription(descriptionIn);
        defaultValue = defaultValueIn;
        basePointerType = BasePointer::StringPointer;
    }
    bool PTLString::ParseFromString(std::string parseVal, void* ptr)
    {
        if (hasBeenParsed) return true;
        *((std::string*)ptr) = parseVal;
        hasBeenParsed = true;
        return true;
    }
    std::string PTLString::GetDefaultValueString(void)
    {
        return defaultValue;
    }
    void PTLString::SetDefaultValue(void* ptr)
    {
        *((std::string*)ptr) = defaultValue;
    }
    void PTLString::Destroy(void){}
}
