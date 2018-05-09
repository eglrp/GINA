
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Own includes

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

#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "BasicFramework.hpp"

//#include "CoordinateFrames.h"
//#include "CoordinateFrameHandler.h"

using namespace std;

int main(int argc, char **argv) {


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
