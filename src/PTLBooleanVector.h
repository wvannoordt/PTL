#ifndef PTLBooleanVector_H
#define PTLBooleanVector_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PTL
{
    class PTLBooleanVector : public InputVariable
    {
        public:
            PTLBooleanVector(std::string description);
            bool ParseFromString(std::string parseVal, void* ptr);
            void Destroy(void);
            void SetDefaultValue(void* ptr);
            std::string GetDefaultValueString(void);
        private:
            PropStringHandler* strHandle;
            std::string defaultValue;
    };
}

#endif
