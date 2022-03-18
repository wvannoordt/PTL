#include <iostream>
#include <string>
#include "Error.h"
#include "PropertyTree.h"
#include "PropStringHandler.h"
#include <fstream>
#include <sys/stat.h>
#include <stack>
#include <vector>
#include <deque>
#include <iterator>
#include <algorithm>
#include <utility>
#include <unistd.h>
namespace PTL
{
    PropertyTree::PropertyTree(void)
    {
        stringHandler = PropStringHandler();
        principalSection = new PropertySection(&stringHandler, 0, NULL);
        principalSection->baseTree = this;
        principalSection->DeclareIsPrincipal();
        principalSection->SetName("[ROOT]");
        closeMessage = "none";
        wasCreatedAsSubtree = false;
    }

    PropertyTree::~PropertyTree(void)
    {
        this->Destroy();
    }

    void PropertyTree::PushSection(std::string pushedSection)
    {
        principalSection = principalSection->PushSection(pushedSection);
        principalSection->DeclareIsPrincipal();
    }

    PropertySection& PropertyTree::Principal(void)
    {
        return *principalSection;
    }

    void PropertyTree::SetAsSubtree(PropertySection& newPrincipal)
    {
        this->Destroy();
        wasCreatedAsSubtree = true;
        principalSection = &newPrincipal;
    }

    void PropertyTree::Destroy(void)
    {
        if (!wasCreatedAsSubtree)
        {
        	if (closeMessage != "none") ptlout << closeMessage << ptl::endl;
            if (principalSection != NULL)
            {
				principalSection->Destroy();
				delete principalSection;
				principalSection = NULL;
            }
        }
    }
    
    std::vector<PropertySection*>::iterator PropertyTree::begin() noexcept
    {
        return principalSection->begin();
    }
    
    std::vector<PropertySection*>::const_iterator PropertyTree::begin() const noexcept
    {
        return principalSection->begin();
    }
    
    std::vector<PropertySection*>::iterator PropertyTree::end() noexcept
    {
        return principalSection->end();
    }
    
    std::vector<PropertySection*>::const_iterator PropertyTree::end() const noexcept
    {
        return principalSection->end();
    }
    
    QueryResult PropertyTree::Query(std::string sectionQuery)
    {
        std::vector<std::string> sptVec = stringHandler.Split(sectionQuery, '.');
        std::reverse(sptVec.begin(), sptVec.end());
        std::stack<std::string> sptStack(std::deque<std::string>(sptVec.begin(), sptVec.end()));
        return principalSection->Query(sptStack, sectionQuery);
    }
    
    void PropertyTree::ResolveAllStrings(void)
    {
        principalSection->ResolveAllStrings();
    }

    void PropertyTree::SetCloseMessage(std::string message)
    {
    	closeMessage = message;
    }

    void PropertyTree::CreateDefaultValuesFile(std::string filename)
    {
        ptlout << "PTL :: Saving default values as " << filename << ptl::endl;
        std::ofstream myfile;
        myfile.open(filename.c_str());
        myfile << stringHandler.GetCommentSpecifier() << " This file was generated with default values" << std::endl;
        principalSection->RecursiveWriteDefaults(myfile);
        myfile.close();
    }
    
    void PropertyTree::Serialize(std::ostream& stream)
    {
        principalSection->RecursiveWriteDefaults(stream);
    }

    void PropertyTree::ReadInputFileToTreeData(std::string filename)
    {
        std::string fileRawContents = stringHandler.Sanitize(stringHandler.ReadFileToString(filename), principalSection->GetContext());
        std::cout << fileRawContents << std::endl;
        principalSection->PopulateInstanceFromString(fileRawContents);
        principalSection->SetValue(fileRawContents);
    }
    
    void PropertyTree::Read(std::string filename)
    {
        ReadInputFileToTreeData(filename);
    }

    void PropertyTree::DebugPrint(void)
    {
        principalSection->DebugPrint();
    }

    void PropertyTree::StrictParse(void)
    {
        if (!principalSection->StrictTraverseParse(""))
        {
            ErrorKill("Found at least 1 invalid argument.");
        }
    }

    PropertySection& PropertyTree::operator [](std::string argument)
    {
        return (*principalSection)[argument];
    }
}
