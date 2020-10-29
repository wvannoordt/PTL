#include "PreProcessContext.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "Error.h"
#include "BuiltInFunctionList.h"

namespace PropTreeLib
{
    PreProcessContext::PreProcessContext(void)
    {
        preProcessIndicator = '#';
        invocationSymbol = "$";
        functionInvocationSymbol = "@";
        invocationStart = "(";
        invocationEnd = ")";
        hostContext = NULL;
        isTopLevelContext = true;
        defineString = "define";
        forbiddenNameChars = "!@#$%^&*(){}[]\\|~\'`?/<>,";
        dlmChar = ' ';
        functionArgDelimiter = ',';
    }

    void PreProcessContext::SetHostContext(PreProcessContext* host)
    {
        hostContext = host;
        isTopLevelContext = false;
    }

    void PreProcessContext::SetStyle(char preProcessIndicator_in, char invocationSymbol_in, char invocationStart_in, char invocationEnd_in)
    {
        preProcessIndicator = preProcessIndicator_in;
        invocationSymbol = invocationSymbol_in;
        invocationStart = invocationStart_in;
        invocationEnd = invocationEnd_in;
    }

    void PreProcessContext::Destroy(void)
    {

    }

    bool PreProcessContext::ValidateDefinition(std::string inputLine)
    {
        if (inputLine.find(preProcessIndicator)==0)
        {
            return true;
        }
        return false;
    }

    bool PreProcessContext::ValidateInvocation(std::string inputLine)
    {
        bool hasVarEval = (inputLine.find(invocationSymbol)!=std::string::npos)&&(inputLine.find(invocationStart)!=std::string::npos)&&(inputLine.find(invocationEnd)!=std::string::npos);
        bool hasFuncEval = StringContains(inputLine, functionInvocationSymbol)&&StringContains(inputLine, invocationStart)&&StringContains(inputLine, invocationEnd);
        return hasVarEval || hasFuncEval;
    }

    bool PreProcessContext::ParseDefinition(std::string inputLine)
    {
        bool error = false;
        std::string errorReason = "none";
        std::string defString = preProcessIndicator + defineString + dlmChar;
        size_t pos = inputLine.find(defString);
        if (pos!=0)
        {
            error = true;
            errorReason = "improper preprocessor formatting";
        }
        std::string valassign = inputLine.substr(defString.length(), inputLine.length()-defString.length());
        pos = valassign.find(dlmChar);
        if (pos==std::string::npos)
        {
            error = true;
            errorReason = "could not find value delimiter";
        }
        std::string varName = valassign.substr(0, pos);
        std::string varVal  = valassign.substr(pos+1,valassign.length()-pos-1);
        if (varName.find_first_of(forbiddenNameChars)!=std::string::npos)
        {
            error = true;
            errorReason = "forbidden character";
        }
        if (varName.length()==0)
        {
            error = true;
            errorReason = "empty variable name";
        }
        if (definitions.find(varName)!=definitions.end())
        {
            error = true;
            errorReason = "multiple definition";
        }
        if (error) ErrorKill("Error parsing preprocessor directive \"" + inputLine + "\": " + errorReason + ".");
        definitions.insert({varName,varVal});
        return true;
    }

    bool PreProcessContext::ParseInvocationExpression(std::string inputLine, std::string* outputLine)
    {
        bool success = true;
        *outputLine = ResolveWithinContext(inputLine, 0, &success);
        if (!success)
        {
            ErrorKill("Error parsing preprocessor invocation \"" + inputLine + "\".");
        }
        return success;
    }

    bool PreProcessContext::StringContains(std::string str, std::string c)
    {
        return (str.find(c) != std::string::npos);
    }

    bool PreProcessContext::SymbolPrecedes(std::string str, std::string preceder, std::string test)
    {
        if (!StringContains(str, preceder)) return false;
        if (!StringContains(str, test)) return true;
        return (str.find(preceder) < str.find(test));
    }

    void PreProcessContext::SplitFunction(std::string str, std::string* pre, std::string* post, std::string* func, std::vector<std::string>* args, int level, bool* success)
    {
        size_t idxFuncSym = str.find(functionInvocationSymbol);
        *pre = str.substr(0, idxFuncSym);
        std::string noPre = str.substr(1+idxFuncSym, str.length()-idxFuncSym-1);
        if (!StringContains(noPre, invocationStart)) ErrorKill("Cannot find arguments for function call " + str);
        size_t iStart = noPre.find(invocationStart);
        size_t iEnd;
        int bracketLevel = 0;
        for (size_t i = iStart; i < noPre.length(); i++)
        {
            if (PositionIsStart(i, noPre, invocationStart)) bracketLevel++;
            if (PositionIsEnd(i, noPre, invocationEnd)) bracketLevel--;
            if (bracketLevel==0) {iEnd = i; break;}
        }
        std::string contents = noPre.substr(iStart+1, iEnd-iStart-1);
        *func = noPre.substr(0, iStart);
        *post = noPre.substr(iEnd+1, noPre.length()-iEnd-1);
        BuildArgs(args, contents, functionArgDelimiter, level, success);
    }

    void PreProcessContext::BuildArgs(std::vector<std::string>* args, std::string line, char delimiterIn, int level, bool* success)
    {
        std::vector<std::string> output;
        std::vector<size_t> positions;
        positions.push_back(0);
        int sectionLevel = 0;
        for (size_t i = 1; i < line.length()-1; i++)
        {
            if (PositionIsStart(i, line, invocationStart))  sectionLevel++;
            if (PositionIsEnd(i, line, invocationEnd)) sectionLevel--;
            if ((line[i]==delimiterIn) && (sectionLevel==0)) positions.push_back(i);
        }
        positions.push_back(line.length());
        for (int i = 0; i < positions.size()-1; i++)
        {
            size_t start = positions[i];
            if (i >0) start++;
            size_t end = positions[i+1];
            std::string elem = line.substr(start, end-start);
            if (elem.length()>0)
            {
                args->push_back(ResolveWithinContext(elem, level+1, success));
            }
        }
    }

    std::string PreProcessContext::EvalFunction(std::string& func, std::vector<std::string>& args)
    {
        if (BuiltIns::builtInFunctions.Exists(func))
        {
            return BuiltIns::builtInFunctions[func](args);
        }
        else
        {
            ErrorKill("Cannot fund built-in function \"" + func + "\"");
            return "NONE";
        }
    }

    std::string PreProcessContext::ResolveWithinContext(std::string str_in, int level, bool* success)
    {
        std::string str = str_in;
        std::string fullInvocationStart = invocationSymbol + invocationStart;
        if (str.length()==0) return str;
        if (StringContains(str, functionInvocationSymbol) && SymbolPrecedes(str, functionInvocationSymbol, invocationSymbol))
        {
            std::string pre, post, func;
            std::vector<std::string> args;
            SplitFunction(str, &pre, &post, &func, &args, level, success);
            return pre + ResolveWithinContext(EvalFunction(func, args), level+1, success) + ResolveWithinContext(post, level+1, success);
        }
        else
        {
            AssertBracketConsistency(str);
            size_t start, end;
            start = str.find(fullInvocationStart);
            if (start==std::string::npos)
            {
                return str;
            }
            int bracketLevel = 1;
            for (size_t i = start+fullInvocationStart.length(); i < str.length(); i++)
            {
                if (PositionIsStart(i, str)||PositionIsStart(i, str, invocationStart)) bracketLevel++;
                if (PositionIsEnd(i, str)||PositionIsEnd(i, str, invocationEnd)) bracketLevel--;
                if (bracketLevel==0) {end = i; break;}
            }
            std::string pre = str.substr(0, start);
            std::string med = str.substr(start+fullInvocationStart.length(), end-start-fullInvocationStart.length());
            std::string post = str.substr(end+invocationEnd.length(), str.length()-end-invocationEnd.length());
            std::string defn = GetDefinition(ResolveWithinContext(med, level+1, success), success);
            std::string after = ResolveWithinContext(post, level+1, success);
            return pre + defn + after;
        }
    }

    std::string PreProcessContext::GetDefinition(std::string input, bool* success)
    {
        std::string macroValue;
        if (!CheckContextDefinition(input, &macroValue))
        {
            ErrorKill("No preprocessor definition found for \"" + input + "\" (is it defined within this context?)");
            *success = false;
        }
        *success = true;
        return macroValue;
    }

    bool PreProcessContext::PositionIsStart(size_t i, std::string str, std::string toFind)
    {
        if (i < toFind.length()-1) return false;
        return (str.substr(i-toFind.length()+1, toFind.length())==toFind);
    }

    bool PreProcessContext::PositionIsStart(size_t i, std::string str)
    {
        std::string fullInvocationStart = invocationSymbol + invocationStart;
        return PositionIsStart(i, str, fullInvocationStart);
    }

    bool PreProcessContext::PositionIsEnd(size_t i, std::string str, std::string toFind)
    {
        if (i+toFind.length()-1 >= (str.length())) return false;
        return (str.substr(i, toFind.length()) == toFind);
    }
    bool PreProcessContext::PositionIsEnd(size_t i, std::string str)
    {
        return PositionIsEnd(i, str, invocationEnd);
    }

    void PreProcessContext::AssertBracketConsistency(std::string str)
    {
        int level = 0;
        for (size_t i = 0; i < str.length(); i++)
        {
            if (PositionIsStart(i, str, invocationStart)) level++;
            if (PositionIsEnd(i, str, invocationEnd))
            {
                level--;
                if (level<0) ErrorKill("Invocation \"" + str + "\" has inconsistent brackets (level < 0).");
            }
        }
        if (level!=0) ErrorKill("Invocation \"" + str + "\" has inconsistent brackets (level != 0).");
    }

    bool PreProcessContext::CheckContextDefinition(std::string input, std::string* output)
    {
        if (definitions.find(input)!=definitions.end())
        {
            *output = definitions[input];
            return true;
        }
        if (isTopLevelContext) return false;
        return hostContext->CheckContextDefinition(input, output);
    }

    std::string PreProcessContext::DebugPrint(void)
    {
        std::string output = "";
        if (definitions.size()==0) return output;
        output = output+"{";
        int i = 0;
        for (std::map<std::string, std::string>::iterator it = definitions.begin(); it!=definitions.end(); it++)
        {
            if (i!=0) output = output + ",";
            output += (it->first + "=" + it->second);
            i++;
        }
        return output+"}";
    }
}
