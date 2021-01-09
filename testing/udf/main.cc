#include "PTL.h"

std::string udf(std::vector<std::string>& args)
{
	std::string output = "UDFVALUE:";
	for (const auto a:args) output += a;
	return output;
}

int main(void)
{
	std::string filename = "udf.ptl";
	PTL::PropertyTree input;
	std::string testValue;
	PTL::AddUserDefinedFunction("udf", udf);
	input["testValue"].MapTo(&testValue) = new PTL::PTLString("default", "Test variable");
	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	std::cout << "UDF Value:\n" << testValue << std::endl;
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
