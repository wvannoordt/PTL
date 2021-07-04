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
        filler = NULL;
        defaultSize = 0;
    }
    PTLBooleanVector::PTLBooleanVector(std::string descriptionIn, int defaultSize_in, bool (*filler_in)(int))
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        basePointerType = BasePointer::BoolVectorPointer;
        filler = filler_in;
        defaultSize = defaultSize_in;
        defaultValue = "[";
        if (defaultSize>0)
        {
            defaultValue+=(filler(0)?"true":"false");
            for (int i = 1; i < defaultSize; i++)
            {
                defaultValue+=",";
                defaultValue+=(filler(i)?"true":"false");
            }
        }
        defaultValue += "]";
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
        if (filler!=NULL)
        {
            for (int i = vec.size(); i < defaultSize; i++)
            {
                vec.push_back(filler(i));
            }
        }
        return true;
    }
    std::string PTLBooleanVector::GetDefaultValueString(void)
    {
        return defaultValue;
    }
    void PTLBooleanVector::SetDefaultValue(void* ptr)
    {
        auto& vec = *((std::vector<bool>*)ptr);
        if (filler==NULL) return;
        for (int i = 0; i < defaultSize; i++)
        {
            vec.push_back(filler(i));
        }
    }
    void PTLBooleanVector::Destroy(void)
    {
        delete strHandle;
    }
}
