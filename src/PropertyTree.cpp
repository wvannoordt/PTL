#include <iostream>
#include <string>
#include "Error.h"
#include "PropertyTree.h"
#include "PropStringHandler.h"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
namespace PropTreeLib
{
    PropertyTree::PropertyTree(void)
    {
        stringHandler = PropStringHandler();
        principalSection = new PropertySection(&stringHandler, 0, NULL);
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

    void PropertyTree::ReadInputFileToTreeData(std::string filename)
    {
        std::string fileRawContents = stringHandler.Sanitize(stringHandler.ReadFileToString(filename), principalSection->GetContext());
        principalSection->PopulateInstanceFromString(fileRawContents);
        principalSection->SetValue(fileRawContents);
    }

    void PropertyTree::DebugPrint(void)
    {
        principalSection->DebugPrint();
    }

    void PropertyTree::StrictParse(void)
    {
        if (!principalSection->StrictTraverseParse(""))
        {
            ErrorKill("Found at least 1 invalid argument. Stopping.");
        }
    }

    PropertySection& PropertyTree::operator [](std::string argument)
    {
        return (*principalSection)[argument];
    }
}
