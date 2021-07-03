#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "vector.ptl";
	PTL::PropertyTree input;

	std::vector<int> intVec;
	std::vector<double> doubleVec;
	std::vector<bool> boolVec;

	input["intvector"].MapTo(&intVec) = new PTL::PTLIntegerVector("int vec");
	input["doublevector"].MapTo(&doubleVec) = new PTL::PTLDoubleVector("double vec");
	input["boolvector"].MapTo(&boolVec) = new PTL::PTLBooleanVector("bool vec");

	input.Read(filename);
	input.StrictParse();
	
	for (auto v:intVec)
	{
		std::cout << v << std::endl;
	}
	
	for (auto v:doubleVec)
	{
		std::cout << v << std::endl;
	}
	
	for (auto v:boolVec)
	{
		std::cout << (v?"true":"false") << std::endl;
	}
	
	input.DebugPrint();
	return 0;
}
