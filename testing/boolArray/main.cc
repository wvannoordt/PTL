#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "static.ptl";
	PTL::PropertyTree input;

	bool* ar1;
	int car1;
	int* static5;
	double* staticDouble;

	input["boolArray"].MapTo(&ar1) = new PTL::PTLStaticBooleanArray(3, "dynamic alloc bool");
	input["staticintarray"].MapTo(&static5) = new PTL::PTLStaticIntegerArray(5, "static alloc int", [](int i){return i*i;});
	input["staticdoublearray"].MapTo(&staticDouble) = new PTL::PTLStaticDoubleArray(3, "static alloc double");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();

	std::cout << "Read boolArray: " << ar1[0] << ", " << ar1[1] << " ..., " << ar1[4] << std::endl;
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
