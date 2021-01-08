#include <iostream>
#include <string>
#include "PTLAutoEnum.h"
#include <map>
#include <vector>

#define MAX_ENUM_OPTIONS 1024


namespace PTL
{
    PTLAutoEnum::PTLAutoEnum(int defaultValueIn, std::string (*stringifier)(int), std::string descriptionIn)
    {
        std::string acceptable = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";
        this->SetDescription(descriptionIn);
        defaultValue = stringifier(defaultValueIn);
        basePointerType = BasePointer::IntPointer;
        for (int i = 0; ((i < MAX_ENUM_OPTIONS)&&(stringifier(i)!=PTL_AUTO_ENUM_TERMINATOR)); i++)
        {
            options.insert({stringifier(i), i});
        }
    }

    std::string PTLAutoEnum::GetAcceptableValueString(void)
    {
        return "";
    }
    bool PTLAutoEnum::ParseFromString(std::string parseVal, void* ptr)
    {
        if (hasBeenParsed) return true;
        if (options.find(parseVal)!=options.end())
        {
            *((int*)ptr) = options[parseVal];
            hasBeenParsed = true;
            return true;
        }
        else
        {
            this->SetDefaultValue(ptr);
            parseErrorString = "Could not match \"" + parseVal + "\" to a valid option. Valid options are:\n";
            for (std::map<std::string, int>::iterator it = options.begin(); it != options.end(); it++)
            {
                parseErrorString += (it->first + "\n");
            }
            return false;
        }
    }
    std::string PTLAutoEnum::GetDefaultValueString(void)
    {
        return defaultValue;
    }
    void PTLAutoEnum::SetDefaultValue(void* ptr)
    {
        *((int*)ptr) = options[defaultValue];
    }
    void PTLAutoEnum::Destroy(void)
    {
        options.clear();
    }
}
