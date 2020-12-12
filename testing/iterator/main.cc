#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "iter.ptl";
	PTL::PropertyTree input;
	input.ReadInputFileToTreeData(filename);
	
	int globalValue;
	input["globalValue"].MapTo(&globalValue) = new PTL::Variables::PTLInteger(3, "Dummy variables");
	
	// for (PTL::Iterator it(input); it.HasNext(); it++)
	// {
	// 
	// }
	
	input.StrictParse();
	input.DebugPrint();

	std::cout << "globalValue: " << globalValue << std::endl;
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
