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
        closeMessage = "none";
    }

    PropertyTree::~PropertyTree(void)
    {
        this->Destroy();
    }

    void PropertyTree::Destroy(void)
    {
    	if (closeMessage != "none") std::cout << closeMessage << std::endl;
        principalSection->Destroy();
        delete principalSection;
    }

    void PropertyTree::SetCloseMessage(std::string message)
    {
    	closeMessage = message;
    }

    void PropertyTree::CreateDefaultValuesFile(std::string filename)
    {
        std::cout << "PTL :: Saving default values as " << filename << std::endl;
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
        principalSection->SetName("[ROOT]");
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
