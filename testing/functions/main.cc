#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "testfile.ptl";
	PTL::PropertyTree input;

	std::string testValue;
	std::string testValue2;
	std::cout << "ENV:" << std::endl;
	input["testValue"].MapTo(&testValue)
	= new PTL::PTLString("default", "Test variable");
	input["testValue2"].MapTo(&testValue2)
	= new PTL::PTLString("default", "Test variable");
	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
