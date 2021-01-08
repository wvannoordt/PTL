#include <iostream>
#include "PTL.h"
int main(int argc, char** argv)
{
	std::string filename = "input.ptl";
	PTL::PropertyTree input;
	PTL::Interactive i(argc, argv, &input);

	double inputValue;

	input["inputValue"].MapTo(&inputValue) = new PTL::PTLDouble(1.0, "A dummy variable");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
