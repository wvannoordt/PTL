#include "InputArguments.h"
#include "Error.h"
namespace PTL
{
    InputArguments::InputArguments(int argc, char** argv)
    {
        for (int i = 0; i < argc; i++)
        {
            if (i!=0) // skip exe
            {
                std::string argStr(argv[i]);
                if (IsOptionArg(argStr))
                {
                    optionArgs.insert({argStr, 0});
                }
                else
                {
                    args.push_back(argStr);
                }
            }
        }
    }
    bool InputArguments::IsOptionArg(std::string str)
    {
        return ((str.length()==2) && (str[0] == '-'));
    }
    bool InputArguments::HasOption(std::string argQuery)
    {
        return (optionArgs.find(argQuery) != optionArgs.end());
    }
    std::string& InputArguments::operator [] (const int i)
    {
        if (i >= args.size()) ErrorKill("Tried to access argument out of range! Index: " + std::to_string(i) + ",  number of arguments: " + std::to_string(args.size()));
        return args[i];
    }
}