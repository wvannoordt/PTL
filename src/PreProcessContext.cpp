#include "PreProcessContext.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "Error.h"

namespace PropTreeLib
{
    PreProcessContext::PreProcessContext(void)
    {
        preProcessIndicator = '#';
        invocationSymbol = "$";
        invocationStart = "(";
        invocationEnd = ")";
        hostContext = NULL;
        isTopLevelContext = true;
        defineString = "define";
        forbiddenNameChars = "!@#$%^&*(){}[]\\|~\'`?/<>,";
        dlmChar = ' ';
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
        return (inputLine.find(invocationSymbol)!=std::string::npos)&&(inputLine.find(invocationStart)!=std::string::npos)&&(inputLine.find(invocationEnd)!=std::string::npos);
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
        bool success;
        *outputLine = ResolveWithinContext(inputLine, 0, &success);
        if (!success)
        {
            ErrorKill("Error parsing preprocessor invocation \"" + inputLine + "\".");
        }
        return success;
    }

    std::string PreProcessContext::ResolveWithinContext(std::string str, int level, bool* success)
    {
        std::string fullInvocationStart = invocationSymbol + invocationStart;
        if (str.length()==0) return str;
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
            if (PositionIsStart(i, str)) bracketLevel++;
            if (PositionIsEnd(i, str)) bracketLevel--;
            if (bracketLevel==0) {end = i; break;}
        }
        std::string pre = str.substr(0, start);
        std::string med = str.substr(start+fullInvocationStart.length(), end-start-fullInvocationStart.length());
        std::string post = str.substr(end+invocationEnd.length(), str.length()-end-invocationEnd.length());
        std::string defn = GetDefinition(ResolveWithinContext(med, level+1, success), success);
        std::string after = ResolveWithinContext(post, level+1, success);
        return pre + defn + after;
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

    bool PreProcessContext::PositionIsStart(size_t i, std::string str)
    {
        std::string fullInvocationStart = invocationSymbol + invocationStart;
        if (i < fullInvocationStart.length()-1) return false;
        return (str.substr(i-fullInvocationStart.length()+1, fullInvocationStart.length())==fullInvocationStart);
    }

    bool PreProcessContext::PositionIsEnd(size_t i, std::string str)
    {
        if (i+invocationEnd.length()-1 >= (str.length())) return false;
        return (str.substr(i, invocationEnd.length()) == invocationEnd);
    }

    void PreProcessContext::AssertBracketConsistency(std::string str)
    {
        int level = 0;
        for (size_t i = 0; i < str.length(); i++)
        {
            if (PositionIsStart(i, str)) level++;
            if (PositionIsEnd(i, str))
            {
                level--;
                if (level<0) ErrorKill("Invocation \"" + str + "\" has inconsistent brackets.");
            }
        }
        if (level!=0) ErrorKill("Invocation \"" + str + "\" has inconsistent brackets.");
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
