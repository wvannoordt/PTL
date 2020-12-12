#include "BuiltInFunctions.h"
#include "Error.h"
#include "PTL.h"
#include "BuiltInUtils.h"
namespace PTL
{
    namespace BuiltIns
    {
        std::string PTLFunc_env(std::vector<std::string>& args)
        {
            char* z = getenv(args[0].c_str());
            if (!z) throw BuiltinException("Invalid environment variable \"" + args[0] + "\"");
            std::string p(z);
            return p;
        }
        
        std::string PTLFunc_cat(std::vector<std::string>& args)
        {
            std::string output = "";
            for (int i = 0; i < args.size(); i++) output = output + args[i];
            return output;
        }
        
        std::string PTLFunc_choose(std::vector<std::string>& args)
        {
            int idx = 0;
            if (args.size() < 2) throw BuiltinException("Not enough arguments (expecting at lest two).");
            try {idx=std::stoi(args[0]);}
            catch (...){throw BuiltinException("Could not parse argument \"" + args[0] + "\" as integer.");}
            if (idx >= args.size()) throw BuiltinException("Index \"" + args[0] + "\" exceeds range of provided arguments (0" + " to " + std::to_string(args.size()-1)+ ")");
            return args[idx+1];
        }
        
        std::string PTLFunc_sum(std::vector<std::string>& args)
        {
            double output = 0;
            for (int i = 0; i < args.size(); i++) output += AssertConvertDouble(args[i]);
            return std::to_string(output);
        }
        
        std::string PTLFunc_prod(std::vector<std::string>& args)
        {
            double output = 1.0;
            for (int i = 0; i < args.size(); i++) output *= AssertConvertDouble(args[i]);
            return std::to_string(output);
        }
        
        std::string PTLFunc_sub(std::vector<std::string>& args)
        {
            double output = 0.0;
            if (args.size() != 2) throw BuiltinException("Expecting exactly two arguments.");
            output = AssertConvertDouble(args[0]);
            output -= AssertConvertDouble(args[1]);
            return std::to_string(output);
        }
        
        std::string PTLFunc_div(std::vector<std::string>& args)
        {
            double output = 1.0;
            if (args.size() != 2) throw BuiltinException("Expecting exactly two arguments.");
            output = AssertConvertDouble(args[0]);
            output /= AssertConvertDouble(args[1]);
            return std::to_string(output);
        }
    }
}