#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "PropertySection.h"
#include "PropStringHandler.h"
#include "ElementClasses.h"
#include "Error.h"
#include "BasePointerTypes.h"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

namespace PTL
{
    PropertySection::PropertySection(PropStringHandler* stringHandler_in, int depthIn, PropertySection* host_in)
    {
        depth = depthIn;
        stringHandler = stringHandler_in;
        wasCreatedFromTemplateDeclaration = false;
        isTerminalNode = false;
        host = host_in;
        templateVariable = NULL;
        terminalEndpointTarget = NULL;
        terminalEndpointTargetSecondaryData = NULL;
        hasValue = false;
        basePointerType = BasePointer::IntPointer;
        isPrincipal = false;
        secondaryBasePointerType = BasePointer::None;
        if (host_in != NULL)
        {
            context.SetHostContext(host->GetContext());
        }
    }

    PreProcessContext* PropertySection::GetContext(void)
    {
        return &context;
    }
    
	std::string PropertySection::GetSectionName(void)
	{
		return sectionName;
	}
    
    PropertySection* PropertySection::PushSection(std::string pushedSection)
    {
        PropertySection* prevHost = host;
        std::string oldName = sectionName;
        this->SetName(pushedSection);
        host = new PropertySection(stringHandler, depth, prevHost);
        host->DeclareIsNotPrincipal();
        if (prevHost!=NULL) prevHost->KeyToNewValue(oldName, host);
        host->NewSection(sectionName, this);
        host->SetName(oldName);
        if (prevHost!=NULL) host->GetContext()->SetHostContext(prevHost->GetContext());
        context.SetHostContext(host->GetContext());
        
        RecursiveIncrementDepth();
        
        return host;
    }
    
    void PropertySection::RecursiveIncrementDepth(void)
    {
        depth++;
        for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
        {
            it->second->RecursiveIncrementDepth();
        }
    }
    
    void PropertySection::NewSection(std::string key, PropertySection* val)
    {
        sectionSubSections.insert({key, val});
        sectionRefs.push_back(val);
    }
    
    void PropertySection::KeyToNewValue(std::string key, PropertySection* newValue)
    {
        std::map<std::string, PropertySection*>::iterator i = sectionSubSections.find(key);
        if (i != sectionSubSections.end())
        {
            i->second = newValue;
        }
        else
        {
            ErrorKill("Error using KeyToNewValue: cannot find key \"" + key + "\"");
        }
    }
    

    void PropertySection::DeclareIsFromTemplateDeclaration(void)
    {
        wasCreatedFromTemplateDeclaration = true;
    }

    void PropertySection::DeclareIsPrincipal(void)
    {
        isPrincipal = true;
    }
    
    void PropertySection::DeclareIsNotPrincipal(void)
    {
        isPrincipal = false;
    }

    void PropertySection::DeclareIsTerminal(void)
    {
        isTerminalNode = true;
    }
    
    std::vector<PropertySection*>::iterator PropertySection::begin() noexcept
    {
        return sectionRefs.begin();
    }
    
    std::vector<PropertySection*>::const_iterator PropertySection::begin() const noexcept
    {
        return sectionRefs.begin();
    }
    
    std::vector<PropertySection*>::iterator PropertySection::end() noexcept
    {
        return sectionRefs.end();
    }
    
    std::vector<PropertySection*>::const_iterator PropertySection::end() const noexcept
    {
        return sectionRefs.end();
    }
    
    QueryResult PropertySection::Query(std::string sectionQuery)
    {
        std::vector<std::string> sptVec = stringHandler->Split(sectionQuery, '.');
        std::reverse(sptVec.begin(), sptVec.end());
        std::stack<std::string> sptStack(std::deque<std::string>(sptVec.begin(), sptVec.end()));
        return this->Query(sptStack, sectionQuery);
    }
    
    QueryResult PropertySection::Query(std::stack<std::string>& sectionQuery, std::string absoluteString)
    {
        std::string toCheck = sectionQuery.top();
        sectionQuery.pop();
        bool hasSection = (sectionSubSections.find(toCheck) == sectionSubSections.end());
        if (hasSection) // didn't find the subsection
        {
            return QueryResult("", false, false, this, absoluteString);
        }
        else
        {
            if (sectionQuery.size() == 0)
            {
                std::string content = "[section content not supported]";
                PropertySection* sec = sectionSubSections[toCheck];
                bool dummy;
                if (sec->isTerminalNode) content = stringHandler->Trim(context.ResolveWithinContext(sec->sectionValue, 0, &dummy));
                return QueryResult(content, true, !(sec->isTerminalNode), sec, absoluteString);
            }
            else
            {
                return sectionSubSections[toCheck]->Query(sectionQuery, absoluteString);
            }
        }
        
    }
    
    std::vector<std::string> PropertySection::GetTerminalSections(void)
    {
        std::vector<std::string> output;
        for (const auto& sec: sectionSubSections)
        {
            if (sec.second->isTerminalNode)
            {
                output.push_back(sec.second->sectionName);
            }
        }
        return output;
    }
    
    void PropertySection::ResolveAllStrings(void)
    {
        if (isTerminalNode)
        {
            bool success = true;
            std::string pre = sectionValue;
            sectionValue = context.ResolveWithinContext(pre, 0, &success);
            if (!success)
            {
                std::string message = "Could not parse the following preprocessor expansion:\n" + pre;
                ErrorKill(message);
            }
        }
        else
        {
            for (const auto sub : sectionSubSections)
            {
                sub.second->ResolveAllStrings();
            }
        }
    }

    void PropertySection::RecursiveWriteDefaults(std::ofstream& myfile)
    {
        std::string indent = "";
        char beginSection, endSection;
        stringHandler->GetSectionStyle(&beginSection, &endSection);
        for (int i = 0; i < depth-1; i++) indent = indent + "    ";
        if (!isTerminalNode)
        {
            if (!isPrincipal)
            {
                myfile << indent << sectionName << ptl::endl;
                myfile << indent << beginSection << ptl::endl;
            }
            for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
            {
                it->second->RecursiveWriteDefaults(myfile);
            }
            if (!isPrincipal)
            {
                myfile << indent << endSection << ptl::endl;
            }
        }
        else
        {
            if (templateVariable==NULL)
            {
                myfile << indent << sectionName << " = "  << sectionValue << ptl::endl;
            }
            else
            {
                myfile << indent << sectionName << " = "  << templateVariable->GetDefaultValueString() << ptl::endl;
            }
        }
    }

    void PropertySection::PopulateInstanceFromString(std::string contents)
    {
        std::vector<std::string> topLevelElements = stringHandler->IdentifyTopLevels(contents);
        for (int i = 0; i < topLevelElements.size(); i++)
        {
            int elementClass = stringHandler->GetElementClass(topLevelElements[i], &context);
            switch (elementClass)
            {
                case EC_VARASSIGN:
                {
                    std::string name, val;
                    stringHandler->ParseElementAsVariable(topLevelElements[i], &name, &val);
                    if (sectionSubSections.find(name)==sectionSubSections.end())
                    {
                        NewSection(name, new PropertySection(stringHandler, depth+1, this));
                    }
                    sectionSubSections[name]->DeclareIsTerminal();
                    sectionSubSections[name]->SetName(name);
                    sectionSubSections[name]->SetValue(val);
                    break;
                }
                case EC_SUBSECTION:
                {
                    std::string name, val;
                    stringHandler->ParseElementAsSubSection(topLevelElements[i], &name, &val);
                    if (sectionSubSections.find(name)==sectionSubSections.end())
                    {
                        NewSection(name, new PropertySection(stringHandler, depth+1, this));
                    }
                    sectionSubSections[name]->PopulateInstanceFromString(val);
                    sectionSubSections[name]->SetName(name);
                    sectionSubSections[name]->SetValue(val);
                    break;
                }
                case EC_PREPROCESS:
                {
                    context.ParseDefinition(topLevelElements[i]);
                    break;
                }
            }
        }
    }

    void PropertySection::SetName(std::string name)
    {
        sectionName = name;
    }

    void PropertySection::SetValue(std::string val)
    {
        sectionValue = val;
        hasValue = true;
    }

    void PropertySection::SetNoValue(void)
    {
        sectionValue = "[DEFAULT]";
        hasValue = false;
    }

    void PropertySection::DebugPrint(void)
    {
        std::string style = "|";
        for (int i = 0; i < depth; i++) style = style + "----";
        if (isTerminalNode)
        {
            ptlout << style << " " << sectionName << " = " << sectionValue << ptl::endl;
        }
        else
        {
            ptlout << style << " " << sectionName << ": " << context.DebugPrint() << ptl::endl;
        }
        for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
        {
            it->second->DebugPrint();
        }

    }

    void PropertySection::Destroy(void)
    {
        if (templateVariable != NULL)
        {
            templateVariable->Destroy();
            delete templateVariable;
        }
        for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
        {
            it->second->Destroy();
            PropertySection* temp = it->second;
            delete temp;
        }
        sectionSubSections.clear();
    }
    
    void PropertySection::StrictParse(void)
    {
    	if (!this->StrictTraverseParse(""))
        {
            ErrorKill("Found at least 1 invalid argument. Stopping.");
        }
    }

    bool PropertySection::StrictTraverseParse(std::string depthString)
    {
        std::string newDepthString;
        if (depth == 1) newDepthString = sectionName;
        else if (depth>1) newDepthString = depthString + "::" + sectionName;
        else newDepthString = "";
        if (!isTerminalNode)
        {
            if (templateVariable != NULL) ErrorKill("A variable template has been assined to a non-terminal section called \"" + newDepthString + "\"");
            bool anyBranchFailed = false;
            for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
            {
                if(!it->second->StrictTraverseParse(newDepthString)) anyBranchFailed = true;
            }
            return !anyBranchFailed;
        }
        else
        {
            if (templateVariable!=NULL) AssertPointerConsistency(newDepthString, false);
            if (terminalEndpointTargetSecondaryData!=NULL) AssertPointerConsistency(newDepthString, true);
            if (templateVariable==NULL)
            {
                ptlout << "Unrecognized variable:" << ptl::endl;
                ptlout << "  >>  " << newDepthString << "  =  " << sectionValue << ptl::endl;
                return false;
            }
            else if(!hasValue)
            {
                templateVariable->SetDefaultValue(terminalEndpointTarget);
                return true;
            }
            else if (context.ValidateInvocation(sectionValue))
            {
                std::string resolvedValue;
                if (context.ParseInvocationExpression(sectionValue, &resolvedValue))
                {
                    sectionValue = resolvedValue;
                    if(!templateVariable->ParseFromString(resolvedValue, terminalEndpointTarget))
                    {
                        ptlout << "Could not parse the following variable (after preprocessor expansion):" << ptl::endl;
                        ptlout << "  >>  " << newDepthString << "  =  " << sectionValue << ptl::endl;
                        templateVariable->PrintFailureMessage();
                        return false;
                    }
                    return true;
                }
            }
            else if(!templateVariable->ParseFromString(sectionValue, terminalEndpointTarget))
            {
                ptlout << "Could not parse the following variable:" << ptl::endl;
                ptlout << "  >>  " << newDepthString << "  =  " << sectionValue << ptl::endl;
                templateVariable->PrintFailureMessage();
                return false;
            }
            if (terminalEndpointTargetSecondaryData!=NULL)
            {
                templateVariable->SetSecondaryVariable(terminalEndpointTargetSecondaryData);
            }
            return true;
        }
    }

    void PropertySection::AssertPointerConsistency(std::string variableLocation, bool isSecondary)
    {
        std::string message;
        if (isSecondary)
        {
            if (!templateVariable->ValidateBasePointer(basePointerType, &message))
            {
                ptlout << "Error in definition of " + variableLocation + ":" << ptl::endl;
                ptlout << message << ptl::endl;
                ErrorKill("Stopping");
            }
        }
        else
        {
            if (!templateVariable->ValidateSecondaryBasePointer(secondaryBasePointerType, &message))
            {
                ptlout << "Error in definition of " + variableLocation + ":" << ptl::endl;
                ptlout << message << ptl::endl;
                ErrorKill("Stopping");
            }
        }
    }

    PropertySection& PropertySection::operator [](std::string argument)
    {
        bool isNewSection = false;
        if (sectionSubSections.find(argument)==sectionSubSections.end())
        {
            isNewSection = true;
            NewSection(argument, new PropertySection(stringHandler, depth+1, this));
        }
        PropertySection* temp = sectionSubSections[argument];
        if (isNewSection)
        {
            temp->SetName(argument);
            temp->SetNoValue();
        }
        temp->DeclareIsFromTemplateDeclaration();
        return *temp;
    }

    PropertySection& PropertySection::operator= (std::string argument)
    {
        this->SetValue(argument);
        this->DeclareIsTerminal();
        return *this;
    }

    std::string PropertySection::GetTotalName(void)
    {
        if (host->isPrincipal) return sectionName;
        else
        {
            return host->GetTotalName() + "::" + sectionName;
        }
    }

    void PropertySection::BreakIfAlreadyMapped(void)
    {
        if (terminalEndpointTarget!=NULL)
        {
            ptlout << "Detected double-mapping of a variable:" << ptl::endl;
            ptlout << GetTotalName() << ptl::endl;
            ErrorKill("Stopping");
        }
    }

    InputVariable* & PropertySection::MapTo(int* ptr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        basePointerType = BasePointer::IntPointer;
        return templateVariable;
    }

    InputVariable* & PropertySection::MapTo(double* ptr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        basePointerType = BasePointer::DoublePointer;
        return templateVariable;
    }

    InputVariable* & PropertySection::MapTo(double** ptr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        basePointerType = BasePointer::DoubleArrayPointer;
        return templateVariable;
    }

    InputVariable* & PropertySection::MapTo(int** ptr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        basePointerType = BasePointer::IntArrayPointer;
        return templateVariable;
    }

    InputVariable* & PropertySection::MapTo(bool* ptr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        basePointerType = BasePointer::BoolPointer;
        return templateVariable;
    }

    InputVariable* & PropertySection::MapTo(std::string* ptr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        basePointerType = BasePointer::StringPointer;
        return templateVariable;
    }

    InputVariable* & PropertySection::MapTo(double** ptr, int* nPtr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        terminalEndpointTargetSecondaryData = (void*)nPtr;
        basePointerType = BasePointer::DoubleArrayPointer;
        secondaryBasePointerType = BasePointer::IntPointer;
        return templateVariable;
    }

    InputVariable* & PropertySection::MapTo(int** ptr, int* nPtr)
    {
        isTerminalNode = true;
        BreakIfAlreadyMapped();
        terminalEndpointTarget = (void*)ptr;
        terminalEndpointTargetSecondaryData = (void*)nPtr;
        basePointerType = BasePointer::IntArrayPointer;
        secondaryBasePointerType = BasePointer::IntPointer;
        return templateVariable;
    }
}
