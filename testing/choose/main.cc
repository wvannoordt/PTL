#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "testfile.ptl";
	PropTreeLib::PropertyTree input;

	std::string testValue1;
	std::string testValue2;
	std::string testValue3;
	std::cout << "ENV:" << std::endl;
	input["testValue1"].MapTo(&testValue1)
	= new PropTreeLib::Variables::PTLString("default", "Test variable");
	input["testValue2"].MapTo(&testValue2)
	= new PropTreeLib::Variables::PTLString("default", "Test variable");
	input["testValue3"].MapTo(&testValue3)
	= new PropTreeLib::Variables::PTLString("default", "Test variable");
	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
