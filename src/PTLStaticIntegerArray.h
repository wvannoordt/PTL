#ifndef PTLStaticIntegerArray_H
#define PTLStaticIntegerArray_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PTL
{
    class PTLStaticIntegerArray : public InputVariable
    {
        public:
            PTLStaticIntegerArray(const int assertCount_in, std::string descriptionIn);
            PTLStaticIntegerArray(const int assertCount_in, std::string descriptionIn, int (*filler_in)(int));
            bool ParseFromString(std::string parseVal, void* ptr);
            void Destroy(void);
            void SetDefaultValue(void* ptr);
            std::string GetDefaultValueString(void);
        private:
            const char* defaultValue;
            bool requiresDelete;
            PropStringHandler* strHandle;
            int* basePtr;
            int count;
            int assertCount;
            int (*filler)(int);
            bool hasFiller;
    };
}

#endif
