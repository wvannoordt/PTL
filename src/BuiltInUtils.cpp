#include "BuiltInUtils.h"

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
    }
}