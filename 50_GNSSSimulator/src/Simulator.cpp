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

GPSEphemerisStore bceStore;
gnsssimulator::TrajectoryStore trajStore;
gnsssimulator::PRsolution prsolution;

typedef map<SatID, Triple> SatDataEpoch;
typedef pair<Triple, SatDataEpoch> SolutionDataBlock;

/// Normal Distribution Error
std::default_random_engine generator(time(0));
std::normal_distribution<double> dist(0.0, 0.01);		// Needs an initial non-NULL value, else throws an exception, we can set a proper value later in the program.

int GNSSSimulator_Process(void);

int main(int argc, char* argv[]) {

	GNSSSimulator_Process();
	return 0;
}


int GNSSSimulator_Process(void) {
	int returnValue = true;
	bool outputfail = false;

	gnsssim_utils gnsssimUtils;

	string trajFileNamewPath = ROOT "\\50_GNSSSimulator\\results\\new_traj.gina";
	string referenceObsFilewPath = ROOT "\\50_GNSSSimulator\\results\\new.18o";

	string navFileNamewPath(ROOT "\\50_GNSSSimulator\\RinexFiles\\brdc0740.18n");//CSsim traj

	ofstream ostrm(ROOT "\\50_GNSSSimulator\\results\\new_Solution.txt", std::ios::out);	//Output file
	ofstream ostrm_sattraj(ROOT "\\50_GNSSSimulator\\results\\new_satTrajectory.txt", std::ios::out);

	PseudoRangeCalculator psdRangeCalc;
	psdRangeCalc.ProcessTrajectoryFile(trajFileNamewPath.c_str());
	psdRangeCalc.ProcessEphemerisFile(navFileNamewPath.c_str());


	GINAParser::TrajectoryStream trajFileInGINA(trajFileNamewPath.c_str(), std::ios::in); //(ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt");
	TrajectoryStream trajFileInTest;
	TrajectoryStream trajFileIn(trajFileNamewPath.c_str());
	GINAParser::TrajectoryHeader trajHeader;
	GINAParser::TrajectoryData trajDataGINA;
	TrajectoryData trajData;
	TrajectoryStore test_trajStore;// = psdRangeCalc.trajStore;

	trajFileInGINA >> trajHeader;

	while (trajFileInGINA >> trajDataGINA) {
		try {
			trajData = trajDataGINA;
			test_trajStore.addPosition(trajData);
		}
		catch (...) {
			cout << "Reading Trajectory data was not successfull " << endl;
		}
	}
	vector<GPSWeekSecond> traj_time = test_trajStore.listTime();

	CommonTime comTime = traj_time[0].convertToCommonTime();
	GPSWeekSecond gpsweeksec(comTime);
	CivilTime civtime(comTime);
	cout << civtime << endl;

	TrajectoryData roverTraj = test_trajStore.findPosition(traj_time[0]);
	Position firstroverPos(roverTraj.pos);

	double out_elevation;
	SatID testId(1, SatID::systemGPS);

	for (auto& time_it : traj_time) {
		civtime = time_it;
		gpsweeksec = civtime.convertToCommonTime();
		vector<double> psdrangeVec;
		vector<SatID> satIdVec;
		vector<double> tropDelays;
		vector<double> ionoParams;
		double psdrange;

		ostrm << "Epoch " << gpsweeksec.week << " " << std::setprecision(12) << gpsweeksec.sow << endl;

		CommonTime comTime_temp = civtime.convertToCommonTime();
		GPSWeekSecond gpsweeksec_temp(comTime_temp);
		TrajectoryData trajPos = test_trajStore.findPosition(gpsweeksec_temp);
		Position roverPos = trajPos.pos;
		Position recpos(roverPos);
		recpos = recpos.asGeodetic();

		PRSolution2 RaimSolver;
		RaimSolver.NSatsReject = 0;

		IonoModel ionoModel;
		psdRangeCalc.getIonoVals(ionoParams);	//Put the 4+4 Ion values into the vector
		ionoModel.setModel(ionoParams.data(), ionoParams.data() + 4);		//.data() tömb pointert ad vissza a vektorból



		ZeroTropModel zeroTrop;			// Null Tropospheric Model
		NeillTropModel neillTrop;		//Basic Neill Tropospheric Model
		neillTrop.setReceiverLatitude(recpos.geodeticLatitude());
		neillTrop.setReceiverHeight(recpos.getHeight());
		neillTrop.setDayOfYear(civtime);

		psdrangeVec.clear();
		satIdVec.clear();
		tropDelays.clear();

		///Psdrangecalc model and error config
		psdRangeCalc.setTropModel(&zeroTrop);				//&neillTrop,&zeroTrop,nullptr
		psdRangeCalc.setIonoModel(nullptr);
		psdRangeCalc.setNormalDIstError(0.0, 0.0);
		/// Error config end

		for (int i = 1; i <= 32; i++) {
			testId.id = i;
			//if (psdRangeCalc.calcPseudoRangeTrop(time_it.convertToCommonTime(), testId, psdrange,&neillTrop)) {
			//if (psdRangeCalc.calcPseudoRangeTropIono(time_it.convertToCommonTime(), testId, psdrange, &neillTrop,&ionoModel)) {
			if (psdRangeCalc.calcPseudoRange(time_it.convertToCommonTime(), testId, psdrange)) {
				psdrangeVec.push_back(psdrange);
				SatID tempid(testId);
				satIdVec.push_back(tempid);

				/// Logging psdrange
				ostrm << tempid << " " << std::setprecision(20) << psdrange << endl;

				/// End of Logging
			}
		}
		if (satIdVec.size() == 0) {
			cout << "Satellite vector contains 0 satellites. Skipping Epoch!" << endl;
			continue;
		}

		TropModel *tropModelPtr = &zeroTrop;
		//TropModel *tropModelPtr = &neillTrop;
		/*TropModel *tropModelPtr;
		if (psdRangeCalc.getTropModel() != nullptr)
		tropModelPtr = &neillTrop;
		else
		tropModelPtr = &zeroTrop;
		*/
		//psdRangeCalc.CalculateTropModelDelays(recpos, comTime_temp, satIdVec, &neillTrop, tropDelays);


		cout << RaimSolver.RAIMCompute(civtime, satIdVec, psdrangeVec, psdRangeCalc.bceStore, tropModelPtr) << endl;
		cout << std::setprecision(12) << RaimSolver.Solution[0] << " " <<
			std::setprecision(12) << RaimSolver.Solution[1] << "  " <<
			std::setprecision(12) << RaimSolver.Solution[2] << endl;
		ostrm << std::setprecision(20) << RaimSolver.Solution[0] << " " << std::setprecision(20) << RaimSolver.Solution[1]
			<< " " << std::setprecision(20) << RaimSolver.Solution[2] << endl;
		cout << "Size of sat vector:" << satIdVec.size() << endl;
		cout << "Number of good satelite are used in the solution: " << RaimSolver.Nsvs << endl;



		Position calculated_roverPos(RaimSolver.Solution[0], RaimSolver.Solution[1], RaimSolver.Solution[2]);

		/*WGS84Ellipsoid* wgs84ellmodel;
		calculated_roverPos.setEllipsoidModel(wgs84ellmodel);*/
		roverPos.setReferenceFrame(ReferenceFrame::WGS84);
		calculated_roverPos.setReferenceFrame(ReferenceFrame::WGS84);
		Position diff;

		diff = roverPos - calculated_roverPos;
		cout << std::setprecision(7) <<
			"Position difference: " << diff.getX() << " " << diff.getY() << " " << diff.getZ() << " Abs: " <<
			sqrt(pow(diff.getX(), 2) + pow(diff.getY(), 2) + pow(diff.getZ(), 2)) << endl;

		ostrm << "Rover " << roverPos.asECEF()[0] << " " << roverPos.asECEF()[1] << " " << roverPos.asECEF()[2] << endl;

		//// Rinex Observation Output generation

		psdRangeCalc.obsContainer[time_it] = make_pair(satIdVec, psdrangeVec);

	}
	gnsssimUtils.prepareRinexObsFile(psdRangeCalc.obsContainer);
	//gnsssimUtils.createRinexObsFile();
	gnsssimUtils.createRinexObsFile(referenceObsFilewPath.c_str());
	ostrm.close();
	ostrm_sattraj.close();
	return true;
}
