#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "static.ptl";
	PTL::PropertyTree input;

	int* ar1;
	int car1;
	int* static5;
	double* staticDouble;

	input["intarray"].MapTo(&ar1, &car1) = new PTL::PTLDynamicIntegerArray("dynamic alloc int");
	input["staticintarray"].MapTo(&static5) = new PTL::PTLStaticIntegerArray(5, "static alloc int", [](int i){return i*i;});
	input["staticdoublearray"].MapTo(&staticDouble) = new PTL::PTLStaticDoubleArray(3, "static alloc double");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();

	std::cout << "Read static5: " << static5[0] << ", " << static5[1] << " ..., " << static5[4] << std::endl;
	std::cout << "Read staticDouble: " << staticDouble[0] << ", ..., " << staticDouble[2] << std::endl;
	std::cout << "Read ar1: " << ar1[0] << ", ..., " << ar1[car1-1] << std::endl;
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
