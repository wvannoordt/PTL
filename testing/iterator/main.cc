#include "PTL.h"

class MyClass : public PTL::PTLGenericObject
{
	using PTL::PTLGenericObject::PTLGenericObject;
	private:
		std::string strval;
		int intval;
	public:
		void Definitions(PTL::PropertySection& section)
		{
			section["name"].MapTo(&strval) = new PTL::PTLString("default value", "The name of the object");
			section["value"].MapTo(&intval) = new PTL::PTLInteger(0, "The value of the object");
		}
		void Print(void)
		{
			std::cout << "name: " << strval << "\nvalue: " << intval << "\nsection: " << objName << std::endl;
		}
};

int main(void)
{
	std::string filename = "iter.ptl";
	PTL::PropertyTree input;
	input.ReadInputFileToTreeData(filename);
	std::cout << "LOOP START" << std::endl;
	std::vector<MyClass> objs;
	for (const auto& s : input["Objects"])
	{
		MyClass obj(s);
		obj.Parse();
		objs.push_back(obj);
	}
	for (auto& o:objs) o.Print();
	std::cout << "LOOP END" << std::endl;
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
