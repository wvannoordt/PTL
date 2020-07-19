#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "static.ptl";
	PropTreeLib::PropertyTree input;

	int* ar1;
	int car1;
	int* static5;
	double* staticDouble;

	input["intarray"].MapTo(&ar1, &car1) = new PropTreeLib::Variables::PTLDynamicIntegerArray("dynamic alloc int");
	input["staticintarray"].MapTo(&static5) = new PropTreeLib::Variables::PTLStaticIntegerArray(5, "static alloc int");
	input["staticdoublearray"].MapTo(&staticDouble) = new PropTreeLib::Variables::PTLStaticDoubleArray(3, "static alloc double");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
