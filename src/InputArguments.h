#ifndef PTL_INPUT_ARGS_H
#define PTL_INPUT_ARGS_H
#include <string>
#include <map>
#include <vector>
namespace PTL
{
    class InputArguments
    {
        public:
            InputArguments(int argc, char** argv);
            std::string& operator [] (const int i);
            bool HasOption(std::string argQuery);
        private:
            bool IsOptionArg(std::string str);
            std::vector<std::string> args;
            std::map<std::string, int> optionArgs;
    };
}
#endif