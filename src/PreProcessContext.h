#ifndef PREPROCESSCONTEXT_H
#define PREPROCESSCONTEXT_H
#include <vector>
#include <map>
#include <string>

namespace PropTreeLib
{
    class PreProcessContext
    {
        public:
            PreProcessContext(void);
            void SetStyle(char preProcessIndicator_in, char invocationSymbol_in, char invocationStart_in, char invocationEnd_in);
            void SetHostContext(PreProcessContext* host);
            void Destroy(void);
            bool ParseDefinition(std::string inputLine);
            bool ValidateDefinition(std::string inputLine);
            bool ValidateInvocation(std::string inputLine);
            bool ParseInvocationExpression(std::string inputLine, std::string* outputLine);
            std::string DebugPrint(void);
            bool CheckContextDefinition(std::string input, std::string* output);
            std::string ResolveWithinContext(std::string str, int level, bool* success);
            void AssertBracketConsistency(std::string str);
            bool PositionIsStart(size_t i, std::string str);
            bool PositionIsEnd(size_t i, std::string str);
            std::string GetDefinition(std::string input, bool* success);
        private:
            std::map<std::string, std::string> definitions;
            char preProcessIndicator, dlmChar;
            std::string invocationStart, invocationEnd, invocationSymbol;
            bool isTopLevelContext;
            PreProcessContext* hostContext;
            std::string defineString, forbiddenNameChars;
    };
}
#endif
