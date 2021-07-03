#ifndef PTLDoubleVector_H
#define PTLDoubleVector_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PTL
{
    class PTLDoubleVector : public InputVariable
    {
        public:
            PTLDoubleVector(std::string description);
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
