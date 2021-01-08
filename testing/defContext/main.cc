#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "input.ptl";
	PTL::PropertyTree input;

	double inputValue1;
	double inputValue2;

	input["inputValue1"].MapTo(&inputValue1) = new PTL::PTLDouble(1.0, "A dummy variable");
	input["inputValue2"].MapTo(&inputValue2) = new PTL::PTLDouble(1.0, "A dummy variable");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
