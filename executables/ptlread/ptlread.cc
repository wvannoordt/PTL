#include "PTL.h"

int main(int argc, char** argv)
{
    bool quiet = false;
    try
    {
        PTL::PropertyTree ptlin;
        PTL::Interactive iobj(argc, argv, &ptlin);
        PTL::InputArguments ptlarg(argc, argv);
        quiet = ptlarg.HasOption("-q");
        std::string targetFile = ptlarg[0];
        std::string contentArgument = ptlarg[1];
        ptlin.ReadInputFileToTreeData(targetFile);
        ptlin.ResolveAllStrings();
        auto qry = ptlin.Query(contentArgument);
        std::cout << qry.Content() << std::endl;
    }
    catch (const PTL::PTLException& e)
    {
        if (!quiet)
        {
            std::cout << e.what() << std::endl;
        }
        return 1;
    }
    return 0;
}