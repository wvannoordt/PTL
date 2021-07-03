#ifndef PTLIntegerVector_H
#define PTLIntegerVector_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PTL
{
    class PTLIntegerVector : public InputVariable
    {
        public:
            PTLIntegerVector(std::string description);
            PTLIntegerVector(std::string descriptionIn, int defaultSize_in, int (*filler_in)(int));
            bool ParseFromString(std::string parseVal, void* ptr);
            void Destroy(void);
            void SetDefaultValue(void* ptr);
            std::string GetDefaultValueString(void);
        private:
            PropStringHandler* strHandle;
            std::string defaultValue;
            int (*filler)(int);
            int defaultSize;
    };
}

#endif
