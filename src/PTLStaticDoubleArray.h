#ifndef PTLStaticDoubleArray_H
#define PTLStaticDoubleArray_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PTL
{
    namespace Variables
    {
        class PTLStaticDoubleArray : public InputVariable
        {
            public:
                PTLStaticDoubleArray(int assertCount_in, std::string descriptionIn);
                PTLStaticDoubleArray(int assertCount_in, std::string descriptionIn, double (*filler_in)(int));
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
            private:
                const char* defaultValue;
                bool requiresDelete;
                PropStringHandler* strHandle;
                double* basePtr;
                int count;
                int assertCount;
                bool hasFiller;
                double (*filler)(int);
        };
    }
}

#endif
