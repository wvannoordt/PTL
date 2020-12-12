#ifndef INPUT_VAR_H
#define INPUT_VAR_H

#include <string>
#include "BasePointerTypes.h"
#include "PTLOutputStream.h"
#include "Error.h"

namespace PTL
{
    namespace Variables
    {
        class InputVariable
        {
            public:
                InputVariable(void){hasRestrictedValues = false; secondaryBasePointerType=None;parseErrorString="none";hasBeenParsed = false;}
                virtual bool ParseFromString(std::string parseVal, void* ptr)=0;
                virtual void Destroy(void)=0;
                virtual std::string GetDescription(void) {return variableDescription;}
                virtual void SetDescription(std::string val) {variableDescription = val;}
                virtual void SetDefaultValue(void* ptr)=0;
                virtual std::string GetDefaultValueString(void)=0;
                virtual void SetSecondaryVariable(void*){};
                void PrintFailureMessage(void)
                {
                    if (parseErrorString!="none")
                    {
                        ptlout << "Info: " << parseErrorString << ptl::endl;
                    }
                }
                virtual std::string GetAcceptableValueString(void){return "[UNSPECIFIED]";};
                virtual void SetName(std::string name) {variableName = name;}
                virtual bool ValidateBasePointer(BasePointer assignedPointer, std::string* message)
                {
                    if (basePointerType != assignedPointer)
                    {
                        *message = "Base pointer error: expecting " + BasePointerToString(basePointerType) + ", but found " + BasePointerToString(assignedPointer) + ".";
                        return false;
                    }
                    *message = "none";
                    return true;
                }
                virtual bool ValidateSecondaryBasePointer(BasePointer assignedPointer, std::string* message)
                {
                    if (secondaryBasePointerType != assignedPointer)
                    {
                        *message = "Secondary base pointer error: expecting " + BasePointerToString(secondaryBasePointerType) + ", but found " + BasePointerToString(assignedPointer) + ".";
                        return false;
                    }
                    *message = "none";
                    return true;
                }
            protected:
                std::string variableDescription;
                std::string variableName;
                std::string parseErrorString;
                BasePointer basePointerType, secondaryBasePointerType;
                bool hasRestrictedValues;
                bool hasBeenParsed;
        };
    }
}

#endif
