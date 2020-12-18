#include "BuiltInFunctions.h"
#include "Error.h"
#include "PTL.h"
#include "BuiltInUtils.h"
#include <cmath>
#define PI180 0.01745329251
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
        
        std::string PTLFunc_fread(std::vector<std::string>& args)
        {
            if (args.size() != 2) throw BuiltinException("Expecting exactly two arguments.");
            int idx = AssertConvertInt(args[1]);
            std::vector<std::string> fileContents = ReadFileToStringVector(args[0]);
            if (idx >= fileContents.size()) throw BuiltinException("Provided index \""
                + std::to_string(idx) + "\" out of range: found only "
                + std::to_string(fileContents.size()) + " lines (zero-base indexing!).");
            return fileContents[idx];
        }
        
        std::string PTLFunc_round(std::vector<std::string>& args)
        {
            if (args.size() != 1) throw BuiltinException("Expecting exactly one argument.");
            double d = AssertConvertDouble(args[0]);
            int output = round(d);
            return std::to_string(output);
        }
        
        std::string PTLFunc_sin(std::vector<std::string>& args)
        {
            if (args.size() != 1) throw BuiltinException("Expecting exactly one argument.");
            double sarg = AssertConvertDouble(args[0]);
            return std::to_string(sin(sarg));
        }

        std::string PTLFunc_cos(std::vector<std::string>& args)
        {
            if (args.size() != 1) throw BuiltinException("Expecting exactly one argument.");
            double sarg = AssertConvertDouble(args[0]);
            return std::to_string(cos(sarg));
        }

        std::string PTLFunc_sind(std::vector<std::string>& args)
        {
            if (args.size() != 1) throw BuiltinException("Expecting exactly one argument.");
            double sarg = AssertConvertDouble(args[0]);
            return std::to_string(sin(PI180*sarg));
        }

        std::string PTLFunc_cosd(std::vector<std::string>& args)
        {
            if (args.size() != 1) throw BuiltinException("Expecting exactly one argument.");
            double sarg = AssertConvertDouble(args[0]);
            return std::to_string(cos(PI180*sarg));
        }

        std::string PTLFunc_atan2d(std::vector<std::string>& args)
        {
            if (args.size() != 2) throw BuiltinException("Expecting exactly two arguments.");
            double yarg = AssertConvertDouble(args[0]);
            double xarg = AssertConvertDouble(args[1]);
            return std::to_string(atan2(yarg, xarg)/PI180);
        }

        std::string PTLFunc_atan2(std::vector<std::string>& args)
        {
            if (args.size() != 2) throw BuiltinException("Expecting exactly two arguments.");
            double yarg = AssertConvertDouble(args[0]);
            double xarg = AssertConvertDouble(args[1]);
            return std::to_string(atan2(yarg, xarg));
        }

        std::string PTLFunc_max(std::vector<std::string>& args)
        {
            double output = 1.0;
            if (args.size() < 1) throw BuiltinException("Expecting at least one argument.");
            output = AssertConvertDouble(args[0]);
            for (int i = 0; i < args.size(); i++)
            {
                double a = AssertConvertDouble(args[i]);
                output = (a>output)?(a):(output);
            }
            return std::to_string(output);
        }

        std::string PTLFunc_min(std::vector<std::string>& args)
        {
            double output = 1.0;
            if (args.size() < 1) throw BuiltinException("Expecting at least one argument.");
            output = AssertConvertDouble(args[0]);
            for (int i = 0; i < args.size(); i++)
            {
                double a = AssertConvertDouble(args[i]);
                output = (a<output)?(a):(output);
            }
            return std::to_string(output);
        }

        std::string PTLFunc_sqrt(std::vector<std::string>& args)
        {
            if (args.size() != 1) throw BuiltinException("Expecting exactly one argument.");
            double sarg = AssertConvertDouble(args[0]);
            return std::to_string(sqrt(sarg));
        }
    }
}