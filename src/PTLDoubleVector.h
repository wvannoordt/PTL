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
            PTLDoubleVector(std::string descriptionIn, int defaultSize_in, double (*filler_in)(int));
            bool ParseFromString(std::string parseVal, void* ptr);
            void Destroy(void);
            void SetDefaultValue(void* ptr);
            std::string GetDefaultValueString(void);
        private:
            PropStringHandler* strHandle;
            std::string defaultValue;
            double (*filler)(int);
            int defaultSize;
    };
}

#endif
