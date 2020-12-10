#include <iostream>
#include <string>
#include "PTLStaticIntegerArray.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLStaticIntegerArray::PTLStaticIntegerArray(const int assertCount_in, std::string descriptionIn)
        {
            strHandle = new PropStringHandler();
            this->SetDescription(descriptionIn);
            defaultValue = "[]";
            basePointerType = BasePointer::IntArrayPointer;
            requiresDelete = false;
            basePtr = NULL;
            count = 0;
            assertCount = assertCount_in;
            basePtr = new int[assertCount_in];
            requiresDelete = true;
            hasFiller = false;
        }
        
        PTLStaticIntegerArray::PTLStaticIntegerArray(const int assertCount_in, std::string descriptionIn, int (*filler_in)(int))
        {
            strHandle = new PropStringHandler();
            this->SetDescription(descriptionIn);
            defaultValue = "[]";
            basePointerType = BasePointer::IntArrayPointer;
            requiresDelete = false;
            basePtr = NULL;
            count = 0;
            assertCount = assertCount_in;
            basePtr = new int[assertCount_in];
            requiresDelete = true;
            hasFiller = true;
            filler = filler_in;
        }
        
        bool PTLStaticIntegerArray::ParseFromString(std::string parseVal, void* ptr)
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
            *((int**)ptr) = basePtr;
            std::string nums = "0123456789";
            std::string parseValCurrent;
            if (assertCount != vals.size())
            {
                parseErrorString = "Expecting exactly " + std::to_string(assertCount) + " elements, but found " + std::to_string(count) + ".";
                return false;
            }
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
                    *(basePtr+j) = i;
                }
                catch (...)
                {
                    this->SetDefaultValue(ptr);
                    parseErrorString = "could not parse at least 1 entry to double.";
                    return false;
                }
            }
            hasBeenParsed = true;
            return true;
        }
        std::string PTLStaticIntegerArray::GetDefaultValueString(void)
        {
            return std::string(defaultValue);
        }
        
        void PTLStaticIntegerArray::SetDefaultValue(void* ptr)
        {
            *((int**)ptr) = basePtr;
            if (hasFiller)
            {
                for (int i = 0; i < assertCount; i++) basePtr[i] = filler(i);
            }
        }
        
        void PTLStaticIntegerArray::Destroy(void)
        {
            delete strHandle;
            if (requiresDelete) delete [] basePtr;
        }
    }
}
