#include "GINAConfig.h"

#include "test_PseudoRangeCalculator.h"
#include <string>
#include "SatID.hpp"

using namespace std;

int PseudoRangeCalculator_test1(void) {
	int returnValue = true;

	string trajFileNamewPath = ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt";

	PseudoRangeCalculator psdRangeCalc;
	psdRangeCalc.ProcessTrajectoryFile(trajFileNamewPath.c_str());

	return returnValue;
}

int PseudoRangeCalculator_test2(void) {
	int returnValue = true;

	string trajFileNamewPath = ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt";
	string navFileNamewPath(ROOT "\\50_GNSSSimulator\\TestFiles\\RINEX_nav\\mobs2530.17n");

	PseudoRangeCalculator psdRangeCalc;
	psdRangeCalc.ProcessTrajectoryFile(trajFileNamewPath.c_str());
	psdRangeCalc.ProcessEphemerisFile(navFileNamewPath.c_str());
	
	return returnValue;
}

int PseudoRangeCalculator_test3(void) {
	int returnValue = true;

	string trajFileNamewPath = ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt";
	string navFileNamewPath(ROOT "\\50_GNSSSimulator\\TestFiles\\RINEX_nav\\brdc2530.17n");

	PseudoRangeCalculator psdRangeCalc;
	psdRangeCalc.ProcessTrajectoryFile(trajFileNamewPath.c_str());
	psdRangeCalc.ProcessEphemerisFile(navFileNamewPath.c_str());



	TrajectoryStream trajFileIn(trajFileNamewPath.c_str()); //(ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt");
	TrajectoryHeader trajHeader;
	TrajectoryData trajData;
	TrajectoryStore test_trajStore;

	trajFileIn >> trajHeader;

	while (trajFileIn >> trajData) {
		try {
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
	Position roverPos(roverTraj.pos);

	double out_elevation;
	SatID testId(1, SatID::systemGPS);
	for (int i = 1; i <= 32; i++) {
		testId.id = i;
		try {
			cout << "Sat " << i << " is visible? " << psdRangeCalc.isSatVisible(roverPos,civtime, testId, out_elevation) << " elavation: " << out_elevation << endl;
		}
		catch (...) {
			cout << "Sat " << i << " is visible? " << "false" << endl;
		}
		
	}

	return returnValue;
}

int PseudoRangeCalculator_test4(void) {
	int returnValue = true;

	string trajFileNamewPath = ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt";
	string navFileNamewPath(ROOT "\\50_GNSSSimulator\\TestFiles\\RINEX_nav\\brdc2530.17n");

	PseudoRangeCalculator psdRangeCalc;
	psdRangeCalc.ProcessTrajectoryFile(trajFileNamewPath.c_str());
	psdRangeCalc.ProcessEphemerisFile(navFileNamewPath.c_str());



	TrajectoryStream trajFileIn(trajFileNamewPath.c_str()); //(ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt");
	TrajectoryHeader trajHeader;
	TrajectoryData trajData;
	TrajectoryStore test_trajStore;

	trajFileIn >> trajHeader;

	while (trajFileIn >> trajData) {
		try {
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
	for (int i = 1; i <= 32; i++) {
		testId.id = i;
		try {
			cout << "Sat " << i << " is visible? " << psdRangeCalc.isSatVisible(firstroverPos, civtime, testId, out_elevation) << " elavation: " << out_elevation << endl;
		}
		catch (...) {
			cout << "Sat " << i << " is visible? " << "false" << endl;
		}

	}

	vector<double> psdrangeVec;
	vector<SatID> satIdVec;
	double psdrange;

	for (int i = 1; i <= 32; i++) {
		testId.id = i;
		if (psdRangeCalc.calcPseudoRange(civtime.convertToCommonTime(), testId, psdrange)) {
			psdrangeVec.push_back(psdrange);
			SatID tempid(testId);
			satIdVec.push_back(tempid);
		}
	}
	
	ofstream ostrm(ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\output_RaimSolution_test.txt", std::ios::out);	//Output file

	PRSolution2 RaimSolver;
	RaimSolver.NSatsReject = 0;

	ZeroTropModel zeroTrop;
	TropModel *tropModelPtr = &zeroTrop;
	cout << RaimSolver.RAIMCompute(civtime, satIdVec, psdrangeVec, psdRangeCalc.bceStore, tropModelPtr) << endl;
	cout << std::setprecision(12) << RaimSolver.Solution[0] << " " <<
		std::setprecision(12) << RaimSolver.Solution[1] << "  " <<
		std::setprecision(12) << RaimSolver.Solution[2] << endl;
	ostrm << std::setprecision(12) << RaimSolver.Solution[0] << " " << std::setprecision(12) << RaimSolver.Solution[1]
		<< " " << std::setprecision(12) << RaimSolver.Solution[2] << endl;
	cout << "Size of sat vector:" << satIdVec.size() << endl;
	cout << "Number of good satelite are used in the solution: " << RaimSolver.Nsvs << endl;

	CommonTime comTime_temp = civtime.convertToCommonTime();
	GPSWeekSecond gpsweeksec_temp(comTime_temp);
	TrajectoryData trajPos = test_trajStore.findPosition(gpsweeksec_temp);
	Position roverPos = trajPos.pos;
	Position calculated_roverPos(RaimSolver.Solution[0], RaimSolver.Solution[1], RaimSolver.Solution[2]);

	/*WGS84Ellipsoid* wgs84ellmodel;
	calculated_roverPos.setEllipsoidModel(wgs84ellmodel);*/
	roverPos.setReferenceFrame(ReferenceFrame::WGS84);
	calculated_roverPos.setReferenceFrame(ReferenceFrame::WGS84);
	Position diff;

	diff = roverPos - calculated_roverPos;
	cout << std::setprecision(7) <<
	"Position difference: " << diff.getX() << " " << diff.getY() << " " << diff.getZ() << endl;

	ostrm.close();

	return true;
}

int PseudoRangeCalculator_test5(void) {
	int returnValue = true;

	string trajFileNamewPath = ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt";
	string navFileNamewPath(ROOT "\\50_GNSSSimulator\\TestFiles\\RINEX_nav\\brdc2530.17n");

	PseudoRangeCalculator psdRangeCalc;
	psdRangeCalc.ProcessTrajectoryFile(trajFileNamewPath.c_str());
	psdRangeCalc.ProcessEphemerisFile(navFileNamewPath.c_str());

	TrajectoryStore trajStore = psdRangeCalc.trajStore;
	GPSEphemerisStore bceStore = psdRangeCalc.bceStore;

	vector<GPSWeekSecond> traj_time = trajStore.listTime();

	CommonTime comTime = traj_time[0].convertToCommonTime();
	GPSWeekSecond gpsweeksec(comTime);
	CivilTime civtime(comTime);
	cout << civtime << endl;

	ZeroTropModel zeroTrop;
	TropModel *tropModelPtr = &zeroTrop;

	vector<double> psdrangeVec;
	vector<SatID> satIdVec;
	
	double psdrange_id3 = 21779924.232;
	double psdrange_id4 = 24878664.571;
	double psdrange_id6 = 24180453.215;
	double psdrange_id7 = 21551889.002;
	double psdrange_id9 = 20816940.309;
	double psdrange_id16 = 21707456.180;
	double psdrange_id22 = 23779143.939;
	double psdrange_id23 = 20423103.937;
	double psdrange_id26 = 23304271.726;
	psdrangeVec.push_back(psdrange_id3);
	psdrangeVec.push_back(psdrange_id4);
	psdrangeVec.push_back(psdrange_id6);
	psdrangeVec.push_back(psdrange_id7);
	psdrangeVec.push_back(psdrange_id9);
	psdrangeVec.push_back(psdrange_id16);
	psdrangeVec.push_back(psdrange_id22);
	psdrangeVec.push_back(psdrange_id23);
	psdrangeVec.push_back(psdrange_id26);

	SatID tempSatId(3, SatID::systemGPS);
	satIdVec.push_back(tempSatId);
	tempSatId.id = 4;
	satIdVec.push_back(tempSatId);
	tempSatId.id = 6;
	satIdVec.push_back(tempSatId);
	tempSatId.id = 7;
	satIdVec.push_back(tempSatId);
	tempSatId.id = 9;
	satIdVec.push_back(tempSatId);
	tempSatId.id = 16;
	satIdVec.push_back(tempSatId);
	tempSatId.id = 22;
	satIdVec.push_back(tempSatId);
	tempSatId.id = 23;
	satIdVec.push_back(tempSatId);
	tempSatId.id = 26;
	satIdVec.push_back(tempSatId);

	PRSolution2 RaimSolver;
	RaimSolver.NSatsReject = 0;

	cout << RaimSolver.RAIMCompute(civtime, satIdVec, psdrangeVec, psdRangeCalc.bceStore, tropModelPtr) << endl;
	cout << std::setprecision(12) << RaimSolver.Solution[0] << " " <<
		std::setprecision(12) << RaimSolver.Solution[1] << "  " <<
		std::setprecision(12) << RaimSolver.Solution[2] << endl;
	cout << "Size of sat vector:" << satIdVec.size() << endl;
	cout << "Number of good satelite are used in the solution: " << RaimSolver.Nsvs << endl;

	CommonTime comTime_temp = civtime.convertToCommonTime();
	GPSWeekSecond gpsweeksec_temp(comTime_temp);
	TrajectoryData trajPos = trajStore.findPosition(gpsweeksec_temp);
	Position roverPos = trajPos.pos;
	Position calculated_roverPos(RaimSolver.Solution[0], RaimSolver.Solution[1], RaimSolver.Solution[2]);

	/*WGS84Ellipsoid* wgs84ellmodel;
	calculated_roverPos.setEllipsoidModel(wgs84ellmodel);*/
	roverPos.setReferenceFrame(ReferenceFrame::WGS84);
	calculated_roverPos.setReferenceFrame(ReferenceFrame::WGS84);
	cout << "Position difference: " << roverPos - calculated_roverPos << endl;

	return returnValue;
}


int PseudoRangeCalculator_test6(void) {
	int returnValue = true;

	string trajFileNamewPath = ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_long.txt";
	//string navFileNamewPath( ROOT "\\50_GNSSSimulator\\TestFiles\\RINEX_nav\\brdc2530.17n");
	string navFileNamewPath(ROOT "\\50_GNSSSimulator\\TestFiles\\RINEX_nav\\brdc3130.17n");//cssim traj

	ofstream ostrm(ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\output_RaimSolution_test.txt", std::ios::out);	//Output file
	ofstream ostrm_sattraj(ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\output_satTrajectory.txt", std::ios::out);

	PseudoRangeCalculator psdRangeCalc;
	psdRangeCalc.ProcessTrajectoryFile(trajFileNamewPath.c_str());
	psdRangeCalc.ProcessEphemerisFile(navFileNamewPath.c_str());



	TrajectoryStream trajFileIn(trajFileNamewPath.c_str()); //(ROOT "\\50_GNSSSimulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_only1.txt");
	TrajectoryHeader trajHeader;
	TrajectoryData trajData;
	TrajectoryStore test_trajStore;

	trajFileIn >> trajHeader;

	while (trajFileIn >> trajData) {
		try {
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
	CivilTime out_civtime;		//For trajectory output
	bool out_trajectory_done = 0;
	
	double out_elevation;
	SatID testId(1, SatID::systemGPS);

	vector<double> psdrangeVec;
	vector<SatID> satIdVec;
	double psdrange;

	PRSolution2 RaimSolver;
	RaimSolver.NSatsReject = 0;
	ZeroTropModel zeroTrop;
	TropModel *tropModelPtr = &zeroTrop;

	///Beginning of Traj_time iteration
	for (auto& time_it : traj_time) {

		civtime = time_it;
		gpsweeksec = civtime.convertToCommonTime();
		cout << civtime << endl;
		ostrm << "Epoch " << gpsweeksec.week << " " << std::setprecision(12) << gpsweeksec.sow << endl;
		//ostrm_sattraj << "Epoch " << gpsweeksec.week << " " << std::setprecision(12) << gpsweeksec.sow << endl;

		satIdVec.clear();
		psdrangeVec.clear();
		for (int i = 1; i <= 32; i++) {
			testId.id = i;
			if (psdRangeCalc.calcPseudoRange(civtime.convertToCommonTime(), testId, psdrange)) {
				psdrangeVec.push_back(psdrange);
				SatID tempid(testId);
				satIdVec.push_back(tempid);

				ostrm << tempid << " " << std::setprecision(12) << psdrange << endl;
				/// Get properly preformatted Trajectory output
				if (!out_trajectory_done) {
					for (auto& out_time : traj_time) {
						out_civtime = out_time;
						Xvt currpos_log = psdRangeCalc.bceStore.getXvt(testId, out_civtime);
						ostrm_sattraj << testId.id << " " << std::setprecision(12) << currpos_log.x[0] << " "
							<< currpos_log.x[1] << " " << currpos_log.x[2] << endl;
					}				
				}
				/// End of Trajectory Log
			}
		}
		out_trajectory_done = 1;

			cout << RaimSolver.RAIMCompute(civtime, satIdVec, psdrangeVec, psdRangeCalc.bceStore, tropModelPtr) << endl;
			cout << std::setprecision(12) << RaimSolver.Solution[0] << " " <<
				std::setprecision(12) << RaimSolver.Solution[1] << "  " <<
				std::setprecision(12) << RaimSolver.Solution[2] << endl;
			ostrm << std::setprecision(20) << RaimSolver.Solution[0] << " " << std::setprecision(20) << RaimSolver.Solution[1]
				<< " " << std::setprecision(20) << RaimSolver.Solution[2] << endl;
			cout << "Size of sat vector:" << satIdVec.size() << endl;
			cout << "Number of good satelite are used in the solution: " << RaimSolver.Nsvs << endl;

			CommonTime comTime_temp = civtime.convertToCommonTime();
			GPSWeekSecond gpsweeksec_temp(comTime_temp);
			TrajectoryData trajPos = test_trajStore.findPosition(gpsweeksec_temp);
			Position roverPos = trajPos.pos;
			Position calculated_roverPos(RaimSolver.Solution[0], RaimSolver.Solution[1], RaimSolver.Solution[2]);

			/*WGS84Ellipsoid* wgs84ellmodel;
			calculated_roverPos.setEllipsoidModel(wgs84ellmodel);*/
			roverPos.setReferenceFrame(ReferenceFrame::WGS84);
			calculated_roverPos.setReferenceFrame(ReferenceFrame::WGS84);
			cout << "Position difference: " << roverPos - calculated_roverPos << " Abs diff: " <<
				sqrt(pow((roverPos[0] - calculated_roverPos[0]), 2) + pow((roverPos[1] - calculated_roverPos[1]), 2) + pow((roverPos[2] - calculated_roverPos[2]), 2))
				<< endl << endl;
			ostrm << "Rover " << roverPos[0] << " " << roverPos[1] << " " << roverPos[2] << endl;
	}
	ostrm.close();
	
	return true;
}
