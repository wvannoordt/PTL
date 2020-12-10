#include <iostream>
#include <string>
#include "PTLDouble.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLDouble::PTLDouble(double defaultValueIn, std::string descriptionIn)
        {
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
            basePointerType = BasePointer::DoublePointer;
            allowed = "0123456789e-.";
        }
        bool PTLDouble::ParseFromString(std::string parseVal, void* ptr)
        {
            if (hasBeenParsed) return true;
            for (size_t a = 0; a < parseVal.length(); a++)
            {
                if (allowed.find(parseVal[a])==std::string::npos)
                {
                    this->SetDefaultValue(ptr);
                    return false;
                }
            }
            double i;
            try
            {
                i=std::stod(parseVal);
                *((double*)ptr) = i;
                hasBeenParsed = true;
                return true;
            }
            catch (...)
            {
                parseErrorString = "could not parse entry to double.";
                this->SetDefaultValue(ptr);
                return false;
            }
            return false;
        }
        std::string PTLDouble::GetDefaultValueString(void)
        {
            return std::to_string(defaultValue);
        }
        void PTLDouble::SetDefaultValue(void* ptr)
        {
            *((double*)ptr) = defaultValue;
        }
        void PTLDouble::Destroy(void){}
    }
}
