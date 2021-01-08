#include <iostream>
#include "PTL.h"
int main(void)
{
	std::string filename = "testfile.in";
	PTL::PropertyTree input;

	bool useWallModel, outputWallModelData;
	int wallModelOutputDataInterval, forcingOffset, rayDim;
	double errorTolerance;
	std::string wallModelOutputDir;
	int momEquation;
	double momEqUnderRelax;
	int turbEquation;
	double turbEqUnderRelax;
	int engyEquation;
	double engyEqUnderRelax;
	double* sampleXLocations;
	int numSampleXLocations;

	input["NavierStokes"]["WallModel"]["useWallModel"].MapTo(&useWallModel)
	= new PTL::PTLBoolean(false, "Flag to enable to wall model");

	input["NavierStokes"]["WallModel"]["outputWallModelData"].MapTo(&outputWallModelData)
	= new PTL::PTLBoolean(false, "Flag to enable wall model output");

	input["NavierStokes"]["WallModel"]["wallModelOutputDataInterval"].MapTo(&wallModelOutputDataInterval)
	= new PTL::PTLInteger(100, "Controls the interval at which wall model data is written");

	input["NavierStokes"]["WallModel"]["forcingOffset"].MapTo(&forcingOffset)
	= new PTL::PTLInteger(4, "Wall model sampling location (number of cells)");

	input["NavierStokes"]["WallModel"]["rayDim"].MapTo(&rayDim)
	= new PTL::PTLInteger(30, "Number of mesh points in wall-normal direction in the wall model");

	input["NavierStokes"]["WallModel"]["errorTolerance"].MapTo(&errorTolerance)
	= new PTL::PTLDouble(1e-6, "Generic error tolerance for iterative procedures in the wall model numerics");

	input["NavierStokes"]["WallModel"]["outputDir"].MapTo(&wallModelOutputDir)
	= new PTL::PTLString(".", "Location for wall model output data");

	input["NavierStokes"]["WallModel"]["SampleXLocations"].MapTo(&sampleXLocations, &numSampleXLocations)
	= new PTL::PTLDynamicDoubleArray("X-locations for inner-solution sampling with precision 1e-5");

	input["NavierStokes"]["WallModel"]["Momentum"]["equation"].MapTo(&momEquation)
	= new PTL::PTLEnum("ODE", "ODE:allmaras:linear", "Specifies what kind of equation to solve for wall model momentum equation");

	input["NavierStokes"]["WallModel"]["Momentum"]["underRelaxation"].MapTo(&momEqUnderRelax)
	= new PTL::PTLDouble(0.4, "Under-relaxation factor for momentum equation Newton solve");

	input["NavierStokes"]["WallModel"]["Turbulence"]["equation"].MapTo(&turbEquation)
	= new PTL::PTLEnum("vanDriest", "ODE:vanDriest:linear", "Specifies what kind of equation to solve for wall model turbulence equation");

	input["NavierStokes"]["WallModel"]["Turbulence"]["underRelaxation"].MapTo(&turbEqUnderRelax)
	= new PTL::PTLDouble(0.4, "Under-relaxation factor for turbulence equation Newton solve");

	input["NavierStokes"]["WallModel"]["Energy"]["equation"].MapTo(&engyEquation)
	= new PTL::PTLEnum("ODE", "ODE:coroccoBuseman:linear", "Specifies what kind of equation to solve for wall model energy equation");

	input["NavierStokes"]["WallModel"]["Energy"]["underRelaxation"].MapTo(&engyEqUnderRelax)
	= new PTL::PTLDouble(0.4, "Under-relaxation factor for energy equation Newton solve");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
