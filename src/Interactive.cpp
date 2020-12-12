#include "Interactive.h"
#include <string>
#include "PTL.h"
#include "PropertyTree.h"
#include "Error.h"
namespace PTL
{
    Interactive::Interactive(int argc_in, char** argv_in, PropertyTree* pTree_in)
    {
        argc = argc_in;
        argv = argv_in;
        pTree = pTree_in;
        ptlOptionSpecification = "PTL:";
        PassArgs();
    }

    void Interactive::Run(void)
    {
        std::string cmd, arg;
        for (int i = 0; i < argc; i++)
        {
            std::string cl(argv[i]);
            ParseArg(cl, &cmd, &arg);
            if (cmd=="defaultVals")
            {
                pTree->CreateDefaultValuesFile(arg);
                EndProgram();
            }
        }
    }
    
    void Interactive::PassArgs(void)
    {
        for (int i = 0; i < argc; i++)
        {
            std::string name, val;
            std::string cl(argv[i]);
            if (ParseCLI(cl, &name, &val))
            {
                pTree->Principal().GetContext()->CreateDefinition(name, val);
            }
        }
    }

    bool Interactive::ParseCLI(std::string cliArg, std::string* varName, std::string* parseableValue)
    {
        *varName = "";
        *parseableValue = "";
        if (cliArg.length() < 2) return false;
        if ((cliArg[0]!='-')||(cliArg[1]!='D')) return false;
        std::string definition = cliArg.substr(2, cliArg.length()-2);
        size_t eq = definition.find("=");
        if (eq==std::string::npos)
        {
            *varName = definition;
            return true;
        }
        std::string key = definition.substr(0, eq);
        std::string val = definition.substr(eq+1, definition.length()-eq);
        *varName = key;
        *parseableValue = val;
        return true;
    }

    void Interactive::EndProgram(void)
    {
        ErrorKill("Terminated program after interactive invocation.");
    }

    void Interactive::ParseArg(std::string line, std::string* cmd, std::string* arg)
    {
        *cmd = "";
        *arg = "";
        if (line.find(ptlOptionSpecification) != std::string::npos)
        {
            std::string pval = line.substr(ptlOptionSpecification.length(), line.length()-ptlOptionSpecification.length());
            size_t n = pval.find("=");
            if (n!=std::string::npos)
            {
                *cmd = pval.substr(0, n);
                *arg = pval.substr(n+1, pval.length()-1-n);
            }
            else
            {
                *cmd = pval;
            }
        }
    }
}
