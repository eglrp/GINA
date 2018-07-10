// Simulator.cpp : Defines the entry point for the console application.
//

#include "GINAConfig.h"

#include "Simulator.h"
#include "Navigation_examples.h"
#include "Test_Trajectory_Class.hpp"
#include "Test_simulatePseudoRange.hpp"
#include "test_PseudoRangeCalculator.h"

#include GINA_PARSER_TrajectoryData_hpp
#include GINA_PARSER_TrajectoryStream_hpp
#include GINA_PARSER_IMUData_hpp

const int ProcessRinex = 1;
const int ProcessTrajectory = 1;
const int gSolution_to_RINEX = 1;
const int Run_Tests = 1;

using namespace gpstk;
using namespace std;


satDataContainer satDataContainer_c;		//Stores C1 and Ephemeris Data for the SVs
GPSEphemerisStore bceStore;
gnsssimulator::TrajectoryStore trajStore;
gnsssimulator::PRsolution prsolution;

typedef map<SatID, Triple> SatDataEpoch;
typedef pair<Triple, SatDataEpoch> SolutionDataBlock;

/// Normal Distribution Error
std::default_random_engine generator(time(0));
std::normal_distribution<double> dist(0.0, 0.01);		// Needs an initial non-NULL value, else throws an exception, we can set a proper value later in the program.


int main(int argc, char* argv[]) {

	PseudoRangeCalculator_test7();
	return 0;
}



bool Test_ReProcess_Solution_Rinex(void)			// TODO: Move to util source
{
	bool test_success = 1;

	Rinex3ObsStream stream;
	Rinex3ObsHeader head;
	Rinex3ObsData data;
	satDataContainer satDataCont_Test;
	SatID satid;
	double pr;
	stream.open((ROOT + string("\\50_GNSSSimulator\\TrajectoryTestFiles\\generatedRINEX.11o")).c_str(), ios::in);

	if (!stream.is_open())
		!test_success;
	try
	{
		stream >> head;
		int indexC1 = head.getObsIndex("C1");

		while (stream >> data) {
			CivilTime civtime(data.time);
			for (auto& it : data.obs) {
				satid = it.first;
				pr = data.getObs(satid, indexC1).data;
				satDataCont_Test.assemblePseudoRangeContainer(satid, civtime, pr);
			}
		}
	}
	catch (...)
	{
		test_success = 0;
	}
	try
	{
		double test_pr, original_pr;
		CivilTime civtime;
		for (auto& satid_it : satDataCont_Test.getSatIDvectorlist()) {
			for (auto& time_it : satDataCont_Test.getEpochVectorforSat(satid_it)) {
				test_pr = satDataCont_Test.getPseudorangeatEpoch(satid_it, time_it);
				original_pr = satDataContainer_c.getPseudorangeatEpoch(satid_it, time_it);
				if (test_pr != original_pr)
					test_success = 0;
			}
			
		}
	}
	catch (...)
	{
		test_success = 0;
	}

	return test_success;
}
