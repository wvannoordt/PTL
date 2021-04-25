#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "stringLiteral.ptl";
	
	PTL::PropertyTree input;
	std::string stringWithSymbols;
	input["stringWithSymbols"].MapTo(&stringWithSymbols) = new PTL::PTLString("Something", "A string literal");

	input.Read(filename);
	input.StrictParse();
	input.DebugPrint();

	std::cout << stringWithSymbols << std::endl;

	return 0;
}
