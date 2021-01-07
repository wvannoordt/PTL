#ifndef PTL_QUERYRESULT_H
#define PTL_QUERYRESULT_H
#include <string>
#include <vector>
namespace PTL
{
    class PropertySection;
    class QueryResult
    {
        public:
            QueryResult(std::string content_in, bool found_in, bool isSection_in, PropertySection* sec_in, std::string absoluteString_in)
            {
                content = content_in;
                found = found_in;
                isSection = isSection_in;
                sec = sec_in;
                absoluteString = absoluteString_in;
            }
            QueryResult(const QueryResult& rhs)
            {
                content = rhs.content;
                found = rhs.found;
                isSection = rhs.isSection;
                sec = rhs.sec;
                absoluteString = rhs.absoluteString;
            }
            operator const std::string();
            operator int ();
            operator std::vector<std::string> ();
            operator double ();
            operator bool ();
            operator std::vector<int> ();
            operator std::vector<double> ();
            std::string Content(void) {return content;}
            bool Found(void) {return found;}
            bool IsSection(void) {return isSection;}
            PropertySection* TreeSection(void) {return sec;}
        private:
            std::string content;
            std::string absoluteString;
            PropertySection* sec;
            bool found, isSection;
            void KillIfNotTerminal(void);
    };
}

#endif