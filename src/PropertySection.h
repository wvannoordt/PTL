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
    class PropertyTree;
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
            template <const size_t cCharSize> PropertySection& operator [] (const char (&key)[cCharSize])
            {
                std::string keyStr(key);
                return (*this)[keyStr];
            }
            template <typename stype> PropertySection& operator = (stype argument)
            {
                std::stringstream st;
                st << argument;
                this->SetValue(st.str());
                this->DeclareIsTerminal();
                return *this;
            }
            InputVariable* & MapTo(int*                 ptr);
            InputVariable* & MapTo(double*              ptr);
            InputVariable* & MapTo(bool*                ptr);
            InputVariable* & MapTo(std::string*         ptr);
            InputVariable* & MapTo(double**             ptr, int* nPtr);
            InputVariable* & MapTo(double**             ptr);
            InputVariable* & MapTo(int**                ptr, int* nPtr);
            InputVariable* & MapTo(int**                ptr);
            InputVariable* & MapTo(bool**               ptr);
            InputVariable* & MapTo(std::vector<int>*    ptr);
            InputVariable* & MapTo(std::vector<double>* ptr);
            InputVariable* & MapTo(std::vector<bool>*   ptr);
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
            PropertyTree& GetBaseTree(void);
            QueryResult Query(std::string sectionQuery);
            QueryResult Query(std::stack<std::string>& sectionQuery, std::string absoluteString);
            std::vector<std::string> GetTerminalSections(void);
            bool HasTemplateVariable(void) {return templateVariable!= NULL;}
            InputVariable* GetTemplateVariable(void) {return templateVariable;}
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
            std::string GetSectionValue(void) {return sectionValue;}
            bool Has(const std::string& queryval)
            {
                std::size_t ip = queryval.find('.');
                if(ip == std::string::npos)
                {
                    return (sectionSubSections.find(queryval) != sectionSubSections.end());
                }
                else
                {
                    std::string term = queryval.substr(0, ip);
                    std::string next = queryval.substr(ip+1, queryval.length() - (ip + 1));
                    if (sectionSubSections.find(term) == sectionSubSections.end())
                    {
                        return false;
                    }
                    else
                    {
                        return (*this)[term].Has(next);
                    }
                }
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
            PropertyTree* baseTree;
            void* terminalEndpointTarget;
            void* terminalEndpointTargetSecondaryData;
            BasePointer basePointerType;
            BasePointer secondaryBasePointerType;
            PreProcessContext context;
            friend class PropertyTree;
    };
}
#endif
