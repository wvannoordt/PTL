#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "input.ptl";
	PTL::PropertyTree input;
	int testVariable;

	input["testVariable"].MapTo(&testVariable) = new PTL::PTLInteger(1, "]");

	input.ReadInputFileToTreeData(filename);
	try
	{
		input.StrictParse();
		input.DebugPrint();
	}
	catch (const PTL::PTLException& e)
	{
		std::cout << "Handled the following error elegantly:" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "THIS IS NOT A REAL ERROR. DO NOT WORRY!!!!" << std::endl;
	}
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
