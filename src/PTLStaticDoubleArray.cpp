#include <iostream>
#include <string>
#include "PTLStaticDoubleArray.h"
namespace PTL
{
    PTLStaticDoubleArray::PTLStaticDoubleArray(int assertCount_in, std::string descriptionIn)
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        if (assertCount_in>0)
        {
            defaultValue = "[0";
            for (int i = 1; i < assertCount_in; i++)
            {
                defaultValue = defaultValue + ",0";
            }
            defaultValue = defaultValue + "]";
        }
        basePointerType = BasePointer::DoubleArrayPointer;
        requiresDelete = false;
        basePtr = NULL;
        count = 0;
        assertCount = assertCount_in;
        basePtr = new double[assertCount_in];
        requiresDelete = true;
        hasFiller = false;
    }
    
    PTLStaticDoubleArray::PTLStaticDoubleArray(int assertCount_in, std::string descriptionIn, double (*filler_in)(int))
    {
        strHandle = new PropStringHandler();
        this->SetDescription(descriptionIn);
        defaultValue = "[]";
        if (assertCount_in>0)
        {
            defaultValue = "["+std::to_string(filler_in(0));
            for (int i = 1; i < assertCount_in; i++)
            {
                defaultValue = defaultValue + ",";
                defaultValue = defaultValue + std::to_string(filler_in(i));
            }
            defaultValue = defaultValue + "]";
        }
        basePointerType = BasePointer::DoubleArrayPointer;
        requiresDelete = false;
        basePtr = NULL;
        count = 0;
        assertCount = assertCount_in;
        basePtr = new double[assertCount_in];
        requiresDelete = true;
        hasFiller = true;
        filler = filler_in;
    }
    
    bool PTLStaticDoubleArray::ParseFromString(std::string parseVal, void* ptr)
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
        *((double**)ptr) = basePtr;
        if (count!=assertCount)
        {
            parseErrorString = "Expecting exactly " + std::to_string(assertCount) + " elements, but found " + std::to_string(count) + ".";
            return false;
        }
        for (int i = 0; i < vals.size(); i++)
        {
            double z;
            try
            {
                z=std::stod(vals[i]);
                basePtr[i] = z;
            }
            catch (...)
            {
                parseErrorString = "could not parse at least 1 entry to double.";
                return false;
            }
        }
        hasBeenParsed = true;
        return true;
    }
    std::string PTLStaticDoubleArray::GetDefaultValueString(void)
    {
        return std::string(defaultValue);
    }
    void PTLStaticDoubleArray::SetDefaultValue(void* ptr)
    {
        *((double**)ptr) = basePtr;
        if (hasFiller)
        {
            for (int i = 0; i < assertCount; i++) basePtr[i] = filler(i);
        }
    }
    void PTLStaticDoubleArray::Destroy(void)
    {
        delete strHandle;
        if (requiresDelete)
        {
            delete [] basePtr;
        }
    }
}
