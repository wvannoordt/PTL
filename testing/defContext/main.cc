#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "input.ptl";
	PropTreeLib::PropertyTree input;

	double inputValue1;
	double inputValue2;

	input["inputValue1"].MapTo(&inputValue1) = new PropTreeLib::Variables::PTLDouble(1.0, "A dummy variable");
	input["inputValue2"].MapTo(&inputValue2) = new PropTreeLib::Variables::PTLDouble(1.0, "A dummy variable");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	
	std::cout << "(SUCCESS)" << std::endl;
	return 1;
}
