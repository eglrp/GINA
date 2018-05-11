#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "GPSWeekSecond.hpp"

#include <Eigen/Dense>

#include "strapdown_ecef.h"
#include "geoparam_wgs84.h"
#include "lib_Frames.h"

#include "Trajectory.h"
#include "IMUSignalGenerator.h"
#include "IMUStore.h"

//Own includes
#include "trajectoryReader.h"
#include "satDataContainer.h"
#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"
#include "PRsolution.h"
#include "utils.h"

#include "..\68_PINAParser\include\TrajectoryParser\TrajectoryData.hpp"

// PINAParser
#include "IMUBase.hpp"
#include "IMUData.hpp"
#include "IMUHeader.hpp"
#include "IMUStream.hpp"
#include "IMUStore.hpp"

#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"


using namespace std;

int main(int argc, char **argv){
	


	// IMU file parser
	PINASimulator::IMUStream imuFileIn("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\PINAParser\\files\\example_IMU_Input.pina");
	PINASimulator::IMUStream imuFileOut("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\PINAParser\\files\\example_IMU_Input_out.pina", std::ios::out);
	PINASimulator::IMUHeader imuHeader;
	PINASimulator::IMUData imuData;

	imuFileIn >> imuHeader;
	imuFileOut << imuHeader;

	imuFileIn >> imuData;
	imuFileOut << imuData;

	imuFileIn >> imuData;
	imuFileOut << imuData;

	imuFileIn >> imuData;
	imuFileOut << imuData;

	imuFileIn.close();
	imuFileOut.close();

	// Trajectory file parser

	PINASimulator::TrajectoryStream trajFileIn("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\PINAParser\\files\\example_trajectory.pina");
	PINASimulator::TrajectoryStream trajFileOut("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\PINAParser\\files\\example_trajectory_out.pina", std::ios::out);
	PINASimulator::TrajectoryHeader trajHeader;
	PINASimulator::TrajectoryData trajData;

	trajFileIn >> trajHeader;
	trajFileOut << trajHeader;

	trajFileIn >> trajData;
	trajFileOut << trajData;

	trajFileIn >> trajData;
	trajFileOut << trajData;

	trajFileIn >> trajData;
	trajFileOut << trajData;

	trajFileIn.close();
	trajFileOut.close();

	getchar();

	return 0;
}
