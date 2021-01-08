#include <iostream>
#include "PTL.h"
int main(int argc, char** argv)
{
    //This is the simplest example of a PTL input.

    //Define an input class
    PTL::PropertyTree input;
    PTL::Interactive i(argc, argv, &input);

    std::string str;
    int aa, ab, ba, bb;
    double* ar;
    int n;

    input["SectionA"]["ValueA"].MapTo(&aa) = new PTL::PTLInteger(1, "aa");
    input["SectionA"]["ValueB"].MapTo(&ab) = new PTL::PTLInteger(1, "ab");
    input["SectionA"]["StringValue"].MapTo(&str) = new PTL::PTLString("defaultVal", "str");
    input["SectionB"]["ValueA"].MapTo(&ba) = new PTL::PTLInteger(1, "ba");
    input["SectionB"]["ValueB"].MapTo(&bb) = new PTL::PTLInteger(1, "bb");
    input["SectionB"]["arr"].MapTo(&ar, &n) = new PTL::PTLDynamicDoubleArray("array");
    i.Run();

    return 0;
}
