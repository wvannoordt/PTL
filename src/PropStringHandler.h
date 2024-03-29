#ifndef PROPSTRINGHANDLER_H
#define PROPSTRINGHANDLER_H
#include <string>
#include <vector>
#include "PreProcessContext.h"
namespace PTL
{
    class PropStringHandler
    {
        public:
            PropStringHandler(void);
            bool fileExists(std::string filename);
            std::string ReadFileToString(std::string filename);
            std::string Sanitize(std::string contents, PreProcessContext* context);
            std::string ProcessLine(std::string line, PreProcessContext* context);
            size_t GetOriginalLine(size_t compactPosition);
            void AssertGroupingConsistency(std::string str);
            std::string charString(char x);
            std::vector<std::string> IdentifyTopLevels(std::string line);
            std::vector<std::string> IdentifyTopLevels(std::string line, char delimiterIn);
            void ParseElementAsVariable(std::string line, std::string* variableName, std::string* variableValue);
            void ParseElementAsSubSection(std::string line, std::string* sectionName, std::string* sectionContents);
            std::string RemoveTrailingDelimiters(std::string str, char delimiterIn);
            int GetElementClass(std::string element, PreProcessContext* context);
            void SetDelimiter(char dlm);
            void SetSectionStyle(char open, char close);
            void SetVectorStyle(char open, char close);
            void GetSectionStyle(char* open, char* close);
            void GetVectorStyle(char* open, char* close);
            std::string GetCommentSpecifier(void);
            std::string RemoveUnprotectedWhiteSpace(const std::string& str);
            std::string Trim(std::string str);
            std::string RemoveLineContinuation(const std::string& str, bool& hadLineContinuation);
            std::vector<std::string> Split(const std::string& str, char c);
        private:
            std::string commentString;
            std::string whiteSpace;
            std::string forbiddenNameChars;
            char openSection;
            char closeSection;
            char openVector;
            char closeVector;
            char assignChar;
            char delimiter;
            char variableSpecification;
            char stringQuoteChar;
            char lineContinuationChar;
            std::vector<size_t> lineBreaks;
    };
}

#endif
