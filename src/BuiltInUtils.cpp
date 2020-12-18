#include "BuiltInUtils.h"
#include <fstream>
namespace PTL
{
    namespace BuiltIns
    {
        double AssertConvertDouble(std::string a)
        {
            double t;
            try {t = std::stod(a);}
            catch(...) {throw BuiltinException("Cannot parse argument \"" + a + "\" as a numeric.");}
            return t;
        }
        int AssertConvertInt(std::string a)
        {
            int t;
            try {t = std::stoi(a);}
            catch(...) {throw BuiltinException("Cannot parse argument \"" + a + "\" as an integer.");}
            return t;
        }
        inline bool FileExists(const std::string& name)
        {
            std::ifstream f(name.c_str());
            return f.good();
        }
        
        std::vector<std::string> ReadFileToStringVector(std::string filename)
        {
            if (!FileExists(filename)) throw BuiltinException("Cannot find file \"" + filename + "\"");
            std::vector<std::string> file;
            std::string line;
            file.clear();
            std::ifstream infile (filename, std::ios_base::in);
            while (getline(infile, line, '\n'))
            {
                file.push_back (line);
            }
            return file;
        }
    }
}