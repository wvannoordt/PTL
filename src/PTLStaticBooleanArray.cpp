#include <iostream>
#include <string>
#include "PTLStaticBooleanArray.h"
namespace PTL
{
    PTLStaticBooleanArray::PTLStaticBooleanArray(int assertCount_in, std::string descriptionIn)
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        if (assertCount_in>0)
        {
            defaultValue = "[false";
            for (int i = 1; i < assertCount_in; i++)
            {
                defaultValue += ",false";
            }
            defaultValue += "]";
        }
        basePointerType = BasePointer::BooleanArrayPointer;
        requiresDelete = false;
        basePtr = NULL;
        count = 0;
        assertCount = assertCount_in;
        basePtr = new bool[assertCount_in];
        requiresDelete = true;
        hasFiller = false;
    }
    
    PTLStaticBooleanArray::PTLStaticBooleanArray(int assertCount_in, std::string descriptionIn, bool (*filler_in)(int))
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        if (assertCount_in>0)
        {
            defaultValue = "["+std::to_string(filler_in(0));
            for (int i = 1; i < assertCount_in; i++)
            {
                defaultValue += ","+std::to_string(filler_in(i));
            }
            defaultValue += "]";
        }
        basePointerType = BasePointer::BooleanArrayPointer;
        requiresDelete = false;
        basePtr = NULL;
        count = 0;
        assertCount = assertCount_in;
        basePtr = new bool[assertCount_in];
        requiresDelete = true;
        hasFiller = true;
        filler = filler_in;
    }
    
    bool PTLStaticBooleanArray::ParseFromString(std::string parseVal, void* ptr)
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
        count = vals.size();
        *((bool**)ptr) = basePtr;
        if (count!=assertCount)
        {
            parseErrorString = "Expecting exactly " + std::to_string(assertCount) + " elements, but found " + std::to_string(count) + ".";
            return false;
        }
        for (int i = 0; i < vals.size(); i++)
        {
            bool val = false;
            if (vals[i] == "true" || vals[i] == "True" || vals[i] == "T" || vals[i] == "t") val = true;
            else if (vals[i] == "false" || vals[i] == "False" || vals[i] == "F" || vals[i] == "f") val = false;
            else
            {
                parseErrorString = "could not parse at least 1 entry to bool.";
                return false;
            }
            basePtr[i] = val;
        }
        hasBeenParsed = true;
        return true;
    }
    std::string PTLStaticBooleanArray::GetDefaultValueString(void)
    {
        return std::string(defaultValue);
    }
    void PTLStaticBooleanArray::SetDefaultValue(void* ptr)
    {
        *((bool**)ptr) = basePtr;
        if (hasFiller)
        {
            for (int i = 0; i < assertCount; i++) basePtr[i] = filler(i);
        }
    }
    void PTLStaticBooleanArray::Destroy(void)
    {
        delete strHandle;
        if (requiresDelete)
        {
            delete [] basePtr;
        }
    }
}
