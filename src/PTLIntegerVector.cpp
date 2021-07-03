#include <iostream>
#include <string>
#include "PTLIntegerVector.h"
namespace PTL
{
    PTLIntegerVector::PTLIntegerVector(std::string descriptionIn)
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        basePointerType = BasePointer::IntVectorPointer;
        defaultSize = 0;
        filler = NULL;
    }
    
    PTLIntegerVector::PTLIntegerVector(std::string descriptionIn, int defaultSize_in, int (*filler_in)(int))
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        basePointerType = BasePointer::IntVectorPointer;
        defaultSize = defaultSize_in;
        filler = filler_in;
    }
    
    bool PTLIntegerVector::ParseFromString(std::string parseVal, void* ptr)
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
        auto& vec = *((std::vector<int>*)ptr);
        for (int j = 0; j < vals.size(); j++)
        {
            int i;
            parseValCurrent = vals[j];
            try
            {
                for (size_t y = 0; y < parseValCurrent.length(); y++)
                {
                    if (nums.find(parseValCurrent[y]) == std::string::npos)
                    {
                        this->SetDefaultValue(ptr);
                        return false;
                    }
                }
                i=std::stoi(parseValCurrent);
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
        if (filler!=NULL)
        {
            for (int i = vec.size(); i < defaultSize; i++)
            {
                vec.push_back(filler(i));
            }
        }
        return true;
    }
    std::string PTLIntegerVector::GetDefaultValueString(void)
    {
        return defaultValue;
    }
    void PTLIntegerVector::SetDefaultValue(void* ptr)
    {
        auto& vec = *((std::vector<int>*)ptr);
        if (filler==NULL) return;
        for (int i = 0; i < defaultSize; i++)
        {
            vec.push_back(filler(i));
        }
    }
    void PTLIntegerVector::Destroy(void)
    {
        delete strHandle;
    }
}
