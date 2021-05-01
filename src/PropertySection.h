#ifndef PROPSECTION_H
#define PROPSECTION_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "PropStringHandler.h"
#include "InputVariable.h"
#include <stack>
#include <algorithm>
#include "BasePointerTypes.h"
#include "PreProcessContext.h"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iterator>
#include "PTLOutputStream.h"
#include "QueryResult.h"
namespace PTL
{
    class PropertySection
    {
        public:
            PropertySection(PropStringHandler* stringHandler_in, int depthIn, PropertySection* host_in);
            void PopulateInstanceFromString(std::string contents);
            void SetName(std::string name);
            void SetValue(std::string val);
            void SetNoValue(void);
            void DebugPrint(void);
            void DeclareIsFromTemplateDeclaration(void);
            void DeclareIsTerminal(void);
            void Destroy(void);
            PreProcessContext* GetContext(void);
            PropertySection& operator [] (std::string argument);
            template <typename stype> PropertySection& operator = (stype argument)
            {
                std::stringstream st;
                st << argument;
                this->SetValue(st.str());
                this->DeclareIsTerminal();
                return *this;
            }
            InputVariable* & MapTo(int*         ptr);
            InputVariable* & MapTo(double*      ptr);
            InputVariable* & MapTo(bool*        ptr);
            InputVariable* & MapTo(std::string* ptr);
            InputVariable* & MapTo(double**     ptr, int* nPtr);
            InputVariable* & MapTo(double**     ptr);
            InputVariable* & MapTo(int**        ptr, int* nPtr);
            InputVariable* & MapTo(int**        ptr);
            InputVariable* & MapTo(bool**       ptr);
            bool StrictTraverseParse(std::string depthString);
            void StrictParse(void);
            void BreakIfAlreadyMapped(void);
            void DeclareIsPrincipal(void);
            void DeclareIsNotPrincipal(void);
            std::string GetTotalName(void);
            void RecursiveWriteDefaults(std::ostream& myfile);
            PropertySection* PushSection(std::string pushedSection);
            void KeyToNewValue(std::string key, PropertySection* newValue);
            void NewSection(std::string key, PropertySection* val);
            void RecursiveIncrementDepth(void);
            std::string GetSectionName(void);
            void ResolveAllStrings(void);
            QueryResult Query(std::string sectionQuery);
            QueryResult Query(std::stack<std::string>& sectionQuery, std::string absoluteString);
            std::vector<std::string> GetTerminalSections(void);
            std::vector<PropertySection*>::iterator begin() noexcept;
            std::vector<PropertySection*>::const_iterator begin() const noexcept;
            std::vector<PropertySection*>::iterator end() noexcept;
            std::vector<PropertySection*>::const_iterator end() const noexcept;
            template <typename convertType> operator convertType ()
            {
                if (!(this->isTerminalNode))
                {
                    ErrorKill("Can't parse a non-terminal node: " + this->GetSectionName());
                }
                QueryResult qresult(this->sectionValue, true, !(this->isTerminalNode), this, "[none]");
                convertType output = qresult;
                return output;
            }
        private:
            void AssertPointerConsistency(std::string variableLocation, bool isSecondary);
            std::map<std::string,PropertySection*> sectionSubSections;
            std::vector<PropertySection*> sectionRefs;
            PropStringHandler* stringHandler;
            int depth;
            bool wasCreatedFromTemplateDeclaration, isTerminalNode, hasValue, isPrincipal;
            PropertySection* host;
            std::string sectionName, sectionValue;
            InputVariable* templateVariable;
            void* terminalEndpointTarget;
            void* terminalEndpointTargetSecondaryData;
            BasePointer basePointerType;
            BasePointer secondaryBasePointerType;
            PreProcessContext context;
    };
}
#endif
