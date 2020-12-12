#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "branches.ptl";
	PTL::PropertyTree input;
	PTL::PropertyTree subtree1;
	PTL::PropertyTree subtree2;

	double v1;
	std::string h, a;
	double* ar;
	int num;

	subtree1.SetAsSubtree(input["Subtree1"]);
	subtree2.SetAsSubtree(input["Subtree2"]);

	subtree1["value1"].MapTo(&v1) = new PTL::Variables::PTLDouble(1.0, "a description");
	subtree1["othervalue"].MapTo(&h) = new PTL::Variables::PTLString("s", "a description");

	subtree2["value2"].MapTo(&a) = new PTL::Variables::PTLString("s", "a description");
	subtree2["anothervalue"].MapTo(&ar, &num) = new PTL::Variables::PTLDynamicDoubleArray("a description");

	input.ReadInputFileToTreeData(filename);
	subtree1.StrictParse();
	subtree2.StrictParse();

	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
