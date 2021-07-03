#include <iostream>
#include <string>
#include "PTLDoubleVector.h"
namespace PTL
{
    PTLDoubleVector::PTLDoubleVector(std::string descriptionIn)
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        basePointerType = BasePointer::IntVectorPointer;
    }
    bool PTLDoubleVector::ParseFromString(std::string parseVal, void* ptr)
    {
        if (hasBeenParsed) return true;
        char open, close;
        strHandle->GetVectorStyle(&open, &close);
        size_t sPos = parseVal.find(open);
        size_t ePos = parseVal.find(close);
        if (ePos-sPos==1) return true; //empty
        if (sPos!=0||ePos!=parseVal.length()-1) return false;
        std::string internalValues = parseVal.substr(sPos+1,ePos-sPos-1);
        std::vector<std::string> vals = strHandle->IdentifyTopLevels(internalValues, ',');
        std::string parseValCurrent;
        auto& vec = *((std::vector<double>*)ptr);
        for (int j = 0; j < vals.size(); j++)
        {
            double i;
            parseValCurrent = vals[j];
            try
            {
                i=std::stod(parseValCurrent);
                vec.push_back(i);
            }
            catch (...)
            {
                parseErrorString = "could not parse at least 1 entry to integer.";
                this->SetDefaultValue(ptr);
                return false;
            }
        }
        hasBeenParsed = true;
        return true;
    }
    std::string PTLDoubleVector::GetDefaultValueString(void)
    {
        return defaultValue;
    }
    void PTLDoubleVector::SetDefaultValue(void* ptr){}
    void PTLDoubleVector::Destroy(void)
    {
        delete strHandle;
    }
}
