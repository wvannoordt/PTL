#include <iostream>
#include <string>
#include "PTLBooleanVector.h"
namespace PTL
{
    PTLBooleanVector::PTLBooleanVector(std::string descriptionIn)
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        basePointerType = BasePointer::BoolVectorPointer;
    }
    bool PTLBooleanVector::ParseFromString(std::string parseVal, void* ptr)
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
        std::string nums = "0123456789";
        std::string parseValCurrent;
        auto& vec = *((std::vector<bool>*)ptr);
        for (int j = 0; j < vals.size(); j++)
        {
            std::string parseVal = vals[j];
            if ((parseVal=="True")||(parseVal=="true")||(parseVal=="T")||(parseVal=="t"))
            {
                vec.push_back(true);
            }
            else if ((parseVal=="False")||(parseVal=="false")||(parseVal=="F")||(parseVal=="f"))
            {
                vec.push_back(false);
            }
            else
            {
                parseErrorString = "could not parse entry to bool.";
                this->SetDefaultValue(ptr);
                return false;
            }
        }
        hasBeenParsed = true;
        return true;
    }
    std::string PTLBooleanVector::GetDefaultValueString(void)
    {
        return defaultValue;
    }
    void PTLBooleanVector::SetDefaultValue(void* ptr){}
    void PTLBooleanVector::Destroy(void)
    {
        delete strHandle;
    }
}
