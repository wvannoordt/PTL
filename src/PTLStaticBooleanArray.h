#ifndef PTLStaticBooleanArray_H
#define PTLStaticBooleanArray_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PTL
{
    class PTLStaticBooleanArray : public InputVariable
    {
        public:
            PTLStaticBooleanArray(int assertCount_in, std::string descriptionIn);
            PTLStaticBooleanArray(int assertCount_in, std::string descriptionIn, bool (*filler_in)(int));
            bool ParseFromString(std::string parseVal, void* ptr);
            void Destroy(void);
            void SetDefaultValue(void* ptr);
            std::string GetDefaultValueString(void);
        private:
            std::string defaultValue;
            bool requiresDelete;
            PropStringHandler* strHandle;
            bool* basePtr;
            int count;
            int assertCount;
            bool hasFiller;
            bool (*filler)(int);
    };
}

#endif
