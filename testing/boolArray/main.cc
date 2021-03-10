#include <iostream>
#include "PTL.h"
std::string boolstr(bool h)
{
	return h?"true":"false";
}

int main(void)
{
	std::string filename = "static.ptl";
	PTL::PropertyTree input;

	bool* ar1;

	input["boolArray"].MapTo(&ar1) = new PTL::PTLStaticBooleanArray(3, "dynamic alloc bool");

	input.Read(filename);
	input.StrictParse();

	std::cout << "Read boolArray: " << boolstr(ar1[0]) << ", " << boolstr(ar1[1]) << ", " << boolstr(ar1[2]) << std::endl;
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
