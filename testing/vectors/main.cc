#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "vector.ptl";
	PTL::PropertyTree input;

	std::vector<int> intVec;
	std::vector<double> doubleVec;
	std::vector<bool> boolVec;

	input["intvector"].MapTo(&intVec) = new PTL::PTLIntegerVector("int vec", 8, [](int i) -> int {return i+15;});
	input["doublevector"].MapTo(&doubleVec) = new PTL::PTLDoubleVector("double vec");
	input["boolvector"].MapTo(&boolVec) = new PTL::PTLBooleanVector("bool vec");
	
	input.Read(filename);
	input.StrictParse();

	for (auto y:intVec)
	{
		std::cout << y << std::endl;
	}

	
	input.DebugPrint();
	return 0;
}
