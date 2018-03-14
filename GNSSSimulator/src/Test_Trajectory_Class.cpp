#include "stdafx.h"
#include "TrajectoryHeader.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryStream.hpp"
#include "RinexNavData.hpp"
#include "TrajectoryStore.hpp"

#include "Test_Trajectory_Class.hpp"

using namespace gpstk::StringUtils;
using namespace std;

void Test_Trajectory_1(void) {

	gnsssimulator::TrajectoryStream trajFileIn("C:\\Users\\LUS2BP\\Source\\Repos\\GNSSSimulator\\Simulator\\TrajectoryTestFiles\\Test1_TrajectoryFileExample.txt");
	gnsssimulator::TrajectoryStream trajFileOut("C:\\Users\\LUS2BP\\Source\\Repos\\GNSSSimulator\\Simulator\\TrajectoryTestFiles\\Test1_TrajectoryFileExample_Out.txt", std::ios::out);
	gnsssimulator::TrajectoryHeader trajHeader;
	
	trajFileIn >> trajHeader;
	trajFileOut << trajHeader;

	trajFileIn.close();
	trajFileOut.close();
}

void Test_Trajectory_2(void) {

	gnsssimulator::TrajectoryStream trajFileIn("..\\Simulator\\TrajectoryTestFiles\\Test2_TrajectoryFileExample.txt");
	gnsssimulator::TrajectoryStream trajFileOut("..\\Simulator\\TrajectoryTestFiles\\Test2_TrajectoryFileExample_Out.txt", std::ios::out);
	gnsssimulator::TrajectoryHeader trajHeader;
	gnsssimulator::TrajectoryData trajData;

	trajFileIn >> trajHeader;
	trajFileOut << trajHeader;

	cout << "Next epoch" << endl;
	while (trajFileIn >> trajData) {
		cout << endl << "Next epoch" << endl;
		trajFileOut << trajData;
	}
}

	void Test_Trajectory_3(void) {

		gnsssimulator::TrajectoryStream trajFileIn("..\\Simulator\\TrajectoryTestFiles\\Test3_TrajectoryFileExample.txt");
		gnsssimulator::TrajectoryStream trajFileOut("..\\Simulator\\TrajectoryTestFiles\\Test3_TrajectoryFileExample_Out.txt", std::ios::out);
		gnsssimulator::TrajectoryHeader trajHeader;
		gnsssimulator::TrajectoryData trajData;
		gnsssimulator::TrajectoryData testPos;
		trajFileIn >> trajHeader;
		trajFileOut << trajHeader;

		gnsssimulator::TrajectoryStore TrajStore;
		cout << "Next epoch" << endl;
		while (trajFileIn >> trajData) {
			TrajStore.addPosition(trajData);
			testPos = TrajStore.findPosition(trajData.gpsTime);
			
			cout << endl << "Next epoch" << endl;
			trajFileOut << trajData;
			cout << testPos;
	}

	trajFileIn.close();
	trajFileOut.close();
}

	void Test_Trajectory_4(void) {
	
		gnsssimulator::TrajectoryStore test_TrajStore;
		gnsssimulator::TrajectoryData trajData, returnTraj;

		gpstk::Position pos;
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 6;
		trajData.gpsTime = gpsTime;
		
		test_TrajStore.addPosition(trajData);
		trajData.~TrajectoryData();
		returnTraj = test_TrajStore.findPosition(gpsTime);
		
		gnsssimulator::TrajectoryData copyTraj(returnTraj);
		if (returnTraj != copyTraj) {
			cout << "Error. Is not the same." << endl;
		}
	}

	void Test_Trajectory_5(void) {

		gnsssimulator::TrajectoryData orginal, modified;
		orginal.pos.setGeodetic(40, 40, 100);
		orginal.gpsTime.week = 1956;
		orginal.gpsTime.sow = 6;
		modified.pos.setGeodetic(40, 41, 100);
		modified.gpsTime.week = 1956;
		modified.gpsTime.sow = 7;

		gnsssimulator::TrajectoryData copyTraj(orginal);
		if (orginal == copyTraj) {
		}
		else {
			cout << "Error. Is not the same." << endl;
		}
		if (orginal == modified) {
			cout << "Error. Is the same but should not be" << endl;
		}

		if (orginal != copyTraj) {
			cout << "Error. Is not the same." << endl;
		}
		if (orginal != modified) {
		}
		else {
			cout << "Error. Is the same but should not be" << endl;
		}
	}

	void Test_Trajectory_6(void) {

			gnsssimulator::TrajectoryStore test_TrajStore;
			gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

			gpstk::Position pos(0,670000,0);
			gpstk::GPSWeekSecond gpsTime;
			gpsTime.week = 1956;
			gpsTime.sow = 1;
			
			trajData1.gpsTime.week = 1956;
			trajData1.gpsTime.sow = 1;
			trajData1 += pos;
			trajData2.gpsTime.week = 1956;
			trajData2.gpsTime.sow = 2;
			trajData2 += pos;
			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 3;
			trajData3 += pos;

			test_TrajStore.addPosition(trajData1);
			test_TrajStore.addPosition(trajData2);
			test_TrajStore.addPosition(trajData3);
			
			returnTraj = test_TrajStore.findPosition(gpsTime);
			if (returnTraj != trajData1){
				cout << "Szar van a palacsintaban. trajData1" << endl;
			}

			gpsTime.sow = 2;
			returnTraj = test_TrajStore.findPosition(gpsTime);
			if (returnTraj != trajData2) {
				cout << "Szar van a palacsintaban. trajData2" << endl;
			}

			gpsTime.sow = 3;
			returnTraj = test_TrajStore.findPosition(gpsTime);
			if (returnTraj != trajData3) {
				cout << "Szar van a palacsintaban. trajData3" << endl;
			}
		}

	void Test_Trajectory_7(void) {

		gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
		gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

		gpstk::Position pos(0, 670000, 0);
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 1;

		trajData1.gpsTime.week = 1956;
		trajData1.gpsTime.sow = 1;
		trajData1 += pos;
		trajData2.gpsTime.week = 1956;
		trajData2.gpsTime.sow = 2;
		trajData2 += pos;
		trajData3.gpsTime.week = 1956;
		trajData3.gpsTime.sow = 3;
		trajData3 += pos;

		test_TrajStore1.addPosition(trajData1);
		test_TrajStore1.addPosition(trajData2);
		test_TrajStore1.addPosition(trajData3);

		test_TrajStore2.addPosition(trajData1);
		test_TrajStore2.addPosition(trajData2);
		test_TrajStore2.addPosition(trajData3);

		if (test_TrajStore1 == test_TrajStore2) {}
		else {
			cout << "Error. The two traj store should be the same." << endl;

		}
	}

	void Test_Trajectory_8(void) {

			gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
			gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

			gpstk::Position pos(0, 670000, 0);
			gpstk::GPSWeekSecond gpsTime;
			gpsTime.week = 1956;
			gpsTime.sow = 1;

			trajData1.gpsTime.week = 1956;
			trajData1.gpsTime.sow = 1;
			trajData1 += pos;
			trajData2.gpsTime.week = 1956;
			trajData2.gpsTime.sow = 2;
			trajData2 += pos;
			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 3;
			trajData3 += pos;

			test_TrajStore1.addPosition(trajData1);
			test_TrajStore1.addPosition(trajData2);
			test_TrajStore1.addPosition(trajData3);

			test_TrajStore2.addPosition(trajData1);
			test_TrajStore2.addPosition(trajData2);

			if (test_TrajStore1 == test_TrajStore2) {
				cout << "Error. The two traj store should be the same." << endl;
			}

			if (test_TrajStore1 == test_TrajStore2){
				cout << "Error. The two traj store should be the same." << endl;
			}

	}

	void Test_Trajectory_9(void) {

			gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
			gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

			gpstk::Position pos(0, 670000, 0);
			gpstk::GPSWeekSecond gpsTime;
			gpsTime.week = 1956;
			gpsTime.sow = 1;

			trajData1.gpsTime.week = 1956;
			trajData1.gpsTime.sow = 1;
			trajData1 += pos;
			trajData2.gpsTime.week = 1956;
			trajData2.gpsTime.sow = 2;
			trajData2 += pos;
			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 3;
			trajData3 += pos;

			test_TrajStore1.addPosition(trajData1);
			test_TrajStore1.addPosition(trajData2);
			test_TrajStore1.addPosition(trajData3);

			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 5;
			trajData3 += pos;
			test_TrajStore2.addPosition(trajData1);
			test_TrajStore2.addPosition(trajData2);
			test_TrajStore2.addPosition(trajData3);

			if (test_TrajStore1 == test_TrajStore2) {
				cout << "Error. The two traj store should be the same." << endl;
			}

			if (test_TrajStore1 == test_TrajStore2) {
				cout << "Error. The two traj store should be the same." << endl;
			}

	}

	void Test_Trajectory_10(void) {

			gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
			gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

			gpstk::Position pos(0, 670000, 0);
			gpstk::GPSWeekSecond gpsTime;
			gpsTime.week = 1956;
			gpsTime.sow = 1;

			trajData1.gpsTime.week = 1956;
			trajData1.gpsTime.sow = 1;
			trajData1 += pos;
			trajData2.gpsTime.week = 1956;
			trajData2.gpsTime.sow = 2;
			trajData2 += pos;
			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 2;
			trajData3 += pos;

			test_TrajStore1.addPosition(trajData1);
			test_TrajStore1.addPosition(trajData2);
			test_TrajStore1.addPosition(trajData3);

			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 2;
			trajData3 += pos;
			test_TrajStore2.addPosition(trajData1);
			test_TrajStore2.addPosition(trajData2);
			test_TrajStore2.addPosition(trajData3);

			if (test_TrajStore1 != test_TrajStore2) {
				cout << "Error. The two traj store should be the same." << endl;
			}

			if (test_TrajStore1 != test_TrajStore2) {
				cout << "Error. The two traj store should be the same." << endl;
			}

	}

	void Test_Trajectory_11(void) {

			gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
			gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

			gpstk::Position pos1(0, 670000, 0);
			gpstk::Position pos2(0, 780001, 0);
			gpstk::GPSWeekSecond gpsTime;
			gpsTime.week = 1956;
			gpsTime.sow = 1;

			trajData1.gpsTime.week = 1956;
			trajData1.gpsTime.sow = 1;
			trajData1 += pos1;
			trajData2.gpsTime.week = 1956;
			trajData2.gpsTime.sow = 2;
			trajData2 += pos1;
			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 3;
			trajData3 += pos1;

			test_TrajStore1.addPosition(trajData1);
			test_TrajStore1.addPosition(trajData2);
			test_TrajStore1.addPosition(trajData3);

			trajData3.gpsTime.week = 1956;
			trajData3.gpsTime.sow = 3;
			trajData3 += pos2;
			test_TrajStore2.addPosition(trajData1);
			test_TrajStore2.addPosition(trajData2);
			test_TrajStore2.addPosition(trajData3);

			if (test_TrajStore1 == test_TrajStore2) {
				cout << "Error. The two traj store should not be the same." << endl;
			}

			if (test_TrajStore1 == test_TrajStore2) {
				cout << "Error. The two traj store should not be the same." << endl;
			}

	}

	void Test_Trajectory_12(void) {

		gpstk::Position pos1(0, 670000, 0);
		gpstk::Position pos2(1, 670001, 2);
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 1;

		gpstk::Position pos_diff = pos1 - pos2;
		cout << pos_diff.getX() << endl;
		cout << pos_diff.getY() << endl;
		cout << pos_diff.getZ() << endl;
	}

	void Test_Trajectory_13(void) {
	
		gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
		gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

		gpstk::Position pos1(0, 670000.0, 0);
		gpstk::Position pos2(0, 670000.0, 0);
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 1;

		trajData1.gpsTime.week = 1956;
		trajData1.gpsTime.sow = 1;
		trajData1 += pos1;
		trajData2.gpsTime.week = 1956;
		trajData2.gpsTime.sow = 2;
		trajData2 += pos1;
		trajData3.gpsTime.week = 1956;
		trajData3.gpsTime.sow = 3;
		trajData3 += pos1;

		test_TrajStore1.addPosition(trajData1);
		test_TrajStore1.addPosition(trajData2);
		test_TrajStore1.addPosition(trajData3);

		test_TrajStore2.addPosition(trajData1);
		test_TrajStore2.addPosition(trajData2);
		test_TrajStore2.addPosition(trajData3);

		if (test_TrajStore1 != test_TrajStore2) {
			cout << "Error. The two traj store should be the same." << endl;
		}

		if (test_TrajStore1 == test_TrajStore2) {}
		else{
			cout << "Error. The two traj store should be the same." << endl;
		}
	}

	void Test_Trajectory_14(void) {

		gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
		gnsssimulator::TrajectoryData trajData11, trajData12, trajData13, trajData21, trajData22, trajData23, returnTraj;

		gpstk::Position pos1(0, 670000.000, 0);
		gpstk::Position pos2(0, 670000.000999, 0);
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 1;

		trajData11.gpsTime.week = 1956;
		trajData11.gpsTime.sow = 1;
		trajData11 += pos1;
		trajData12.gpsTime.week = 1956;
		trajData12.gpsTime.sow = 2;
		trajData12 += pos1;
		trajData13.gpsTime.week = 1956;
		trajData13.gpsTime.sow = 3;
		trajData13 += pos1;

		trajData21.gpsTime.week = 1956;
		trajData21.gpsTime.sow = 1;
		trajData21 += pos2;
		trajData22.gpsTime.week = 1956;
		trajData22.gpsTime.sow = 2;
		trajData22 += pos2;
		trajData23.gpsTime.week = 1956;
		trajData23.gpsTime.sow = 3;
		trajData23 += pos2;

		test_TrajStore1.addPosition(trajData11);
		test_TrajStore1.addPosition(trajData12);
		test_TrajStore1.addPosition(trajData13);

		test_TrajStore2.addPosition(trajData21);
		test_TrajStore2.addPosition(trajData22);
		test_TrajStore2.addPosition(trajData23);

		if (test_TrajStore1 != test_TrajStore2) {
			cout << "Error. The two traj store should be the same." << endl;
		}

		if (test_TrajStore1 == test_TrajStore2) {}
		else {
			cout << "Error. The two traj store should be the same." << endl;
		}
	}

	void Test_Trajectory_15(void) {

		gnsssimulator::TrajectoryStore test_TrajStore1, test_TrajStore2;
		gnsssimulator::TrajectoryData trajData11, trajData12, trajData13, trajData21, trajData22, trajData23, returnTraj;

		gpstk::Position pos1(0, 670000.000, 0);
		gpstk::Position pos2(0, 670000.001, 0);
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 1;

		trajData11.gpsTime.week = 1956;
		trajData11.gpsTime.sow = 1;
		trajData11 += pos1;
		trajData12.gpsTime.week = 1956;
		trajData12.gpsTime.sow = 2;
		trajData12 += pos1;
		trajData13.gpsTime.week = 1956;
		trajData13.gpsTime.sow = 3;
		trajData13 += pos1;

		trajData21.gpsTime.week = 1956;
		trajData21.gpsTime.sow = 1;
		trajData21 += pos2;
		trajData22.gpsTime.week = 1956;
		trajData22.gpsTime.sow = 2;
		trajData22 += pos2;
		trajData23.gpsTime.week = 1956;
		trajData23.gpsTime.sow = 3;
		trajData23 += pos2;

		test_TrajStore1.addPosition(trajData11);
		test_TrajStore1.addPosition(trajData12);
		test_TrajStore1.addPosition(trajData13);

		test_TrajStore2.addPosition(trajData21);
		test_TrajStore2.addPosition(trajData22);
		test_TrajStore2.addPosition(trajData23);

		if (test_TrajStore1 == test_TrajStore2) {
			cout << "Error. The two traj store should not be the same." << endl;
		}

		if (test_TrajStore1 != test_TrajStore2) {}
		else {
			cout << "Error. The two traj store should not be the same." << endl;
		}
	}

	void Test_Trajectory_16(void) {

		gnsssimulator::TrajectoryStore test_TrajStore;
		gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

		gpstk::Position pos1(0, 670000.000, 0);
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 1;

		trajData1.gpsTime.week = 1956;
		trajData1.gpsTime.sow = 1;
		trajData1 += pos1;
		trajData2.gpsTime.week = 1956;
		trajData2.gpsTime.sow = 2;
		trajData2 += pos1;
		trajData3.gpsTime.week = 1956;
		trajData3.gpsTime.sow = 3;
		trajData3 += pos1;

		test_TrajStore.addPosition(trajData1);
		test_TrajStore.addPosition(trajData2);
		test_TrajStore.addPosition(trajData3);

		// Creating the reference vector with the stored times
		gnsssimulator::TrajectoryData trajData4(trajData1), trajData5(trajData2), trajData6(trajData3);

		vector<gpstk::GPSWeekSecond> referenceVector;
		referenceVector.push_back(trajData4.gpsTime);
		referenceVector.push_back(trajData5.gpsTime);
		referenceVector.push_back(trajData6.gpsTime);

		vector<gpstk::GPSWeekSecond> timeVector;

		timeVector = test_TrajStore.listTime();

		if (timeVector != referenceVector) {
			cout << "Error. The two time vector should be the same" << endl;
		}

	}


	void Test_Trajectory_17(void) {

		gnsssimulator::TrajectoryStore test_TrajStore;
		gnsssimulator::TrajectoryData trajData1, trajData2, trajData3, returnTraj;

		gpstk::Position pos1(0, 670000.000, 0);
		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = 1956;
		gpsTime.sow = 1;

		trajData1.gpsTime.week = 1956;
		trajData1.gpsTime.sow = 1;
		trajData1 += pos1;
		trajData2.gpsTime.week = 1956;
		trajData2.gpsTime.sow = 2;
		trajData2 += pos1;
		trajData3.gpsTime.week = 1956;
		trajData3.gpsTime.sow = 3;
		trajData3 += pos1;

		test_TrajStore.addPosition(trajData1);
		test_TrajStore.addPosition(trajData2);
		test_TrajStore.addPosition(trajData3);

		// Creating the reference vector with the stored times
		trajData3.gpsTime.week = 1956;
		trajData3.gpsTime.sow = 300;

		gnsssimulator::TrajectoryData trajData4(trajData1), trajData5(trajData2), trajData6(trajData3);

		vector<gpstk::GPSWeekSecond> referenceVector;
		referenceVector.push_back(trajData4.gpsTime);
		referenceVector.push_back(trajData5.gpsTime);
		referenceVector.push_back(trajData6.gpsTime);

		vector<gpstk::GPSWeekSecond> timeVector;

		timeVector = test_TrajStore.listTime();

		if (timeVector == referenceVector) {
			cout << "Error. The two time vector should not be the same" << endl;
		}

	}


	