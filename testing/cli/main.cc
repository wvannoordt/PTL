#include <iostream>
#include "PropTreeLib.h"
int main(int argc, char** argv)
{
	std::string filename = "input.ptl";
	PropTreeLib::PropertyTree input;
	PropTreeLib::Interactive i(argc, argv, &input);

	double inputValue;

	input["inputValue"].MapTo(&inputValue) = new PropTreeLib::Variables::PTLDouble(1.0, "A dummy variable");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
