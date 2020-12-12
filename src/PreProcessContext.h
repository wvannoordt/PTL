#ifndef PREPROCESSCONTEXT_H
#define PREPROCESSCONTEXT_H
#include <vector>
#include <map>
#include <string>

namespace PTL
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
            std::string ResolveWithinContext(std::string str_in, int level, bool* success);
            void AssertBracketConsistency(std::string str);
            void BuildArgs(std::vector<std::string>* args, std::string line, char delimiterIn, int level, bool* success);
            bool PositionIsStart(size_t i, std::string str, std::string toFind);
            bool PositionIsStart(size_t i, std::string str);
            bool PositionIsEnd(size_t i, std::string str, std::string toFind);
            bool PositionIsEnd(size_t i, std::string str);
            std::string GetDefinition(std::string input, bool* success);
            bool StringContains(std::string str, std::string c);
            bool SymbolPrecedes(std::string str, std::string preceder, std::string test);
            void SplitFunction(std::string str, std::string* pre, std::string* post, std::string* func, std::vector<std::string>* args, int level, bool* success);
            std::string EvalFunction(std::string& func, std::vector<std::string>& args, std::string origLine);
            void CreateDefinition(std::string nameValue, std::string resolvableValue);
        private:
            std::map<std::string, std::string> definitions;
            char preProcessIndicator, dlmChar, functionArgDelimiter;
            std::string invocationStart, invocationEnd, invocationSymbol, functionInvocationSymbol;
            bool isTopLevelContext;
            PreProcessContext* hostContext;
            std::string defineString, forbiddenNameChars;
    };
}
#endif
