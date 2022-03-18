#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "stringLiteral.ptl";
	
	PTL::PropertyTree input;
	std::string stringWithSymbols;
	std::vector<double> dvec;
	input["Section"]["stringWithSymbols"].MapTo(&stringWithSymbols) = new PTL::PTLString("Something", "A string literal");
	input["Section"]["doubleVec"].MapTo(&dvec) = new PTL::PTLDoubleVector("Vector of doubles");
	
	input.Read(filename);
	input.StrictParse();
	input.DebugPrint();
	
	std::cout << stringWithSymbols << std::endl;
	for (auto& i: dvec) std::cout << i << std::endl;

	return 0;
}
