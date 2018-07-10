
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Own includes

#include "IMUControlBase.hpp"
#include "IMUControlData.hpp"

#include "GINAConfig.h"

#include "IMUControlHeader.hpp"
#include "IMUControlStream.hpp"

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


	// IMU Control file parser
	GINAParser::IMUControlStream imuControlFileIn(ROOT "\\68_GINAParser\\files\\example_IMU_Control_Input.gina");
	GINAParser::IMUControlStream imuControlFileOut(ROOT "\\68_GINAParser\\files\\example_IMU_Control_Input_out.gina", std::ios::out);
	GINAParser::IMUControlHeader imuControlHeader;
	GINAParser::IMUControlData imuControlData;

	imuControlFileIn >> imuControlHeader;
	imuControlFileOut << imuControlHeader;

	imuControlFileIn >> imuControlData;
	imuControlFileOut << imuControlData;

	imuControlFileIn >> imuControlData;
	imuControlFileOut << imuControlData;

	imuControlFileIn >> imuControlData;
	imuControlFileOut << imuControlData;

	imuControlFileIn.close();
	imuControlFileOut.close();

	// IMU file parser
	GINAParser::IMUStream imuFileIn(ROOT "\\68_GINAParser\\files\\example_IMU_Input.gina");
	GINAParser::IMUStream imuFileOut(ROOT "\\68_GINAParser\\files\\example_IMU_Input_out.gina", std::ios::out);
	GINAParser::IMUHeader imuHeader;
	GINAParser::IMUData imuData;

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
	
	GINAParser::TrajectoryStream trajFileIn(ROOT "\\68_GINAParser\\files\\example_trajectory.gina");
	GINAParser::TrajectoryStream trajFileOut(ROOT "\\68_GINAParser\\files\\example_trajectory_out.gina", std::ios::out);
	GINAParser::TrajectoryHeader trajHeader;
	GINAParser::TrajectoryData trajData;

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
