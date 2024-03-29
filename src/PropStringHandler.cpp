#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include "PropStringHandler.h"
#include "ElementClasses.h"
#include "Error.h"
#include <locale>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stack>
#include "PreProcessContext.h"
namespace PTL
{
    PropStringHandler::PropStringHandler(void)
    {
        commentString = "//";
        whiteSpace = " \t\f\v\r";
        openSection = '{';
        closeSection = '}';
        openVector = '[';
        closeVector = ']';
        variableSpecification = '$';
        assignChar = '=';
        delimiter = ';';
        stringQuoteChar = '\"';
        forbiddenNameChars = "!@#$%^&*(){}[]\\|~\'`?/<>,";
        lineContinuationChar = '\\';
    }
    std::vector<std::string> PropStringHandler::Split(const std::string& str, char c)
    {
        std::vector<std::string> output;
        std::string elem = "";
        for (int i = 0; i < str.length(); i++)
        {
            char cur = str[i];
            if (cur==c)
            {
                output.push_back(elem);
                elem = "";
            }
            else
            {
                elem += cur;
            }
        }
        output.push_back(elem);
        return output;
    }
    void PropStringHandler::SetDelimiter(char dlm)
    {
        delimiter = dlm;
    }
    void PropStringHandler::SetSectionStyle(char open, char close)
    {
        closeSection = close;
        openSection = open;
    }
    void PropStringHandler::SetVectorStyle(char open, char close)
    {
        closeVector = close;
        openVector = open;
    }
    std::string PropStringHandler::GetCommentSpecifier(void)
    {
        return commentString;
    }
    void PropStringHandler::GetSectionStyle(char* open, char* close)
    {
        *close = closeSection;
        *open =  openSection;
    }
    void PropStringHandler::GetVectorStyle(char* open, char* close)
    {
        *close = closeVector;
        *open =  openVector;
    }
    bool PropStringHandler::fileExists(std::string filename)
    {
        std::ifstream infile(filename.c_str());
        return infile.good();
    }
    std::string PropStringHandler::ReadFileToString(std::string filename)
    {
        if (!fileExists(filename)) ErrorKill("file " + filename + " cannot be accessed.");
        std::ifstream f(filename);
        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }
    std::string PropStringHandler::Sanitize(std::string contents, PreProcessContext* context)
    {
        std::string output = "";
        contents = contents + "\n";
        bool done = false;
        size_t begin = 0;
        size_t end = contents.find("\n", begin);
        while (end!=std::string::npos)
        {
            std::string line = ProcessLine(contents.substr(begin, end-begin), context);
            output = output + line;
            lineBreaks.push_back(line.length());
            begin = end+1;
            end = contents.find("\n", begin);
        }
        std::string outputWithDelimitedSections = "";
        for (size_t i = 0; i < output.length(); i++)
        {
            if (output[i]==closeSection) {outputWithDelimitedSections = outputWithDelimitedSections + output[i] + delimiter;}
            else {outputWithDelimitedSections = outputWithDelimitedSections + output[i];}
        }
        AssertGroupingConsistency(outputWithDelimitedSections);
        return outputWithDelimitedSections;
    }

    std::vector<std::string> PropStringHandler::IdentifyTopLevels(std::string line)
    {
        return IdentifyTopLevels(line, delimiter);
    }
    
    std::vector<std::string> PropStringHandler::IdentifyTopLevels(std::string line, char delimiterIn)
    {
        std::vector<std::string> output;
        std::vector<size_t> positions;
        positions.push_back(0);
        int sectionLevel = 0;
        int vectorLevel  = 0;
        for (size_t i = 1; i < line.length()-1; i++)
        {
            if (line[i]==openSection)  sectionLevel++;
            if (line[i]==closeSection) sectionLevel--;
            if (line[i]==openVector)   vectorLevel ++;
            if (line[i]==closeVector)  vectorLevel --;
            if ((line[i]==delimiterIn) && (sectionLevel==0) && (vectorLevel==0)) positions.push_back(i);
        }
        positions.push_back(line.length());
        for (int i = 0; i < positions.size()-1; i++)
        {
            size_t start = positions[i];
            if (i >0) start++;
            size_t end = positions[i+1];
            std::string elem = RemoveTrailingDelimiters(line.substr(start, end-start), delimiterIn);
            if (elem.length()>0) output.push_back(elem);
        }
        return output;
    }

    std::string PropStringHandler::RemoveTrailingDelimiters(std::string str, char delimiterIn)
    {
        size_t i;
        for (i = 0; str[str.length()-i-1]==delimiterIn;i++){}
        return str.substr(0, str.length()-i);
    }

    void PropStringHandler::AssertGroupingConsistency(std::string str)
    {
        int sectionLevel = 0;
        int vectorLevel = 0;
        std::stack<size_t> openSectionPositions;
        std::stack<size_t> openVectorPositions;
        for (size_t i = 0; i < str.length(); i++)
        {
            if (str[i]==openSection)
            {
                openSectionPositions.push(i);
                sectionLevel++;
                if (vectorLevel!=0) ErrorKill("Error near line " + std::to_string(GetOriginalLine(i)) + ": cannot begin section inside vector declaration.");
            }
            if (str[i]==closeSection)
            {
                sectionLevel--;
                if (sectionLevel<0) ErrorKill("Error near line " + std::to_string(GetOriginalLine(i)) + ": extra section terminator \"" + charString(closeSection) +"\".");
                openSectionPositions.pop();
                if (vectorLevel!=0) ErrorKill("Error near line " + std::to_string(GetOriginalLine(i)) + ": cannot end section inside vector declaration.");
            }
            if (str[i]==openVector)
            {
                openVectorPositions.push(i);
                vectorLevel++;
            }
            if (str[i]==closeVector)
            {
                vectorLevel--;
                if (vectorLevel<0)  ErrorKill("Error near line " + std::to_string(GetOriginalLine(i)) + ": extra vector terminator \"" + charString(closeVector) +"\".");
                openVectorPositions.pop();
            }
        }
        if (!openSectionPositions.empty())
        {
            std::string er = std::to_string(GetOriginalLine(openSectionPositions.top()));
            openSectionPositions.pop();
            while (!openSectionPositions.empty()){er = er + ", " + std::to_string(GetOriginalLine(openSectionPositions.top()));openSectionPositions.pop();}
            ErrorKill("Unresolved section initializer \"" + charString(openSection) + "\" near the following lines: " + er + ".");
        }
        if (!openVectorPositions.empty())
        {
            std::string er = std::to_string(GetOriginalLine(openVectorPositions.top()));
            while (!openVectorPositions.empty()){er = er + ", " + std::to_string(GetOriginalLine(openVectorPositions.top()));openVectorPositions.pop();}
            ErrorKill("Unresolved vector initializer \"" + charString(openVector) + "\" near the following lines: " + er + ".");
        }
    }

    std::string PropStringHandler::ProcessLine(std::string line, PreProcessContext* context)
    {
        std::string inter = Trim(line);
        size_t commentPosition = line.find(commentString);
        if (commentPosition != std::string::npos) inter = line.substr(0, commentPosition);
        std::string output = "";
        bool lineContainsAssignment = false;
        bool isdef = context->ValidateDefinition(inter);
        //check if the current line ends in a line break
        bool lineHasLineContinuation = false;
        inter = Trim(inter);
        inter = this->RemoveLineContinuation(inter, lineHasLineContinuation);
        if (isdef)
        {
            lineContainsAssignment = true;
            output = output+inter;
        }
        else
        {
            for (size_t i = 0; i < inter.length(); i++)
            {
                lineContainsAssignment = lineContainsAssignment || (inter[i] == assignChar);
            }
            // std::cout << inter << std::endl;
            output = this->RemoveUnprotectedWhiteSpace(inter);
            // std::cout << output << std::endl;
            // std::cin.get();
        }
        if (lineContainsAssignment && !lineHasLineContinuation) output += delimiter;
        return output;
    }
    std::string PropStringHandler::RemoveUnprotectedWhiteSpace(const std::string& str)
    {
        std::string output = "";
        bool quote_escaped = false;
        for (size_t i = 0; i < str.length(); i++)
        {
            if (quote_escaped && str[i]=='\'') quote_escaped = false;
            bool isWhiteSpace = (whiteSpace.find(str[i]) != std::string::npos);
            if (!isWhiteSpace || quote_escaped)
            {
                if (str[i] != '\'') output += str[i];
            }
            // else
            // {
            //     break;
            // }
            if (!quote_escaped && str[i]=='\'') quote_escaped = true;
        }
        return output;
    }
    std::string PropStringHandler::RemoveLineContinuation(const std::string& str, bool& hadLineContinuation)
    {
        hadLineContinuation = false;
        if (str.length()>1)
        {
            hadLineContinuation = (str[str.length()-1]==this->lineContinuationChar);
        }
        if (hadLineContinuation)
        {
            std::string output = "";
            for (std::size_t i = 0; i < str.length()-1; i++) output += str[i];
            return output;
        }
        else
        {
            return str;
        } 
    }

    std::string PropStringHandler::Trim(std::string str)
    {
        size_t start, end;
        for (start = 0; start<str.length(); start++) {if (whiteSpace.find(str[start]) == std::string::npos) {break;}}
        for (end = str.length()-1; end >= 0; end--) {if (whiteSpace.find(str[end]) == std::string::npos) {break;}}
        std::string output = str.substr(start, end+1-start);
        return output;
    }

    size_t PropStringHandler::GetOriginalLine(size_t compactPosition)
    {
        size_t accSize = 0;
        size_t i = 0;
        for (; i < lineBreaks.size(); i++)
        {
            accSize += lineBreaks[i];
            if (accSize > compactPosition) return i+1;
        }
        return 0;
    }

    int PropStringHandler::GetElementClass(std::string element, PreProcessContext* context)
    {
        if (element.find(charString(openSection)) != std::string::npos) return EC_SUBSECTION;
        else if (context->ValidateDefinition(element)) return EC_PREPROCESS;
        return EC_VARASSIGN;
    }

    void PropStringHandler::ParseElementAsVariable(std::string line, std::string* variableName, std::string* variableValue)
    {
        size_t assignpos = line.find(charString(assignChar));
        if (assignpos == std::string::npos) ErrorKill("Error parsing element \"" + line + "\": cannot find assignment operator \"" + charString(assignChar) + "\"");
        *variableName = line.substr(0, assignpos);
        if (variableName->find_first_of(forbiddenNameChars) != std::string::npos) ErrorKill("Error parsing element \"" + line + "\": forbidden character in name (possible forgotten delimiter?).");
        *variableValue = line.substr(assignpos+1, line.length()-1-assignpos);
    }

    void PropStringHandler::ParseElementAsSubSection(std::string line, std::string* sectionName, std::string* sectionContents)
    {
        size_t openPos = line.find_first_of(charString(openSection));
        if ((openPos == std::string::npos)) ErrorKill("Error parsing element \"" + line + "\": broken section delimiters (opening). This error should not be possible.");
        int bLevel = 1;
        size_t closePos = 0;
        bool found = false;
        for (size_t i = openPos+1; i < line.length(); i++)
        {
            if (line[i]==openSection)  bLevel++;
            if (line[i]==closeSection) bLevel--;
            if (bLevel==0)
            {
                found = true;
                closePos = i;
                break;
            }
        }
        if (!found) ErrorKill("Error parsing element \"" + line + "\": broken section delimiters. This error should not be possible.");
        *sectionName = line.substr(0, openPos);
        if (sectionName->find_first_of(forbiddenNameChars) != std::string::npos) ErrorKill("Error parsing element \"" + line + "\": forbidden character in section name (possible forgotten delimiter?).");
        *sectionContents = line.substr(openPos+1, closePos-openPos-1);
    }

    std::string PropStringHandler::charString(char x)
    {
        std::string s(1, x);
        return s;
    }
}
