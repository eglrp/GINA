#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linalg.h"
#include "ConcreteDataProcess.h"
#include "Data.h"
#include "PinaFile.h"
#include "GPSWeekSecond.hpp"
#include "InsTkFunctions.h"

#include <Eigen/Dense>

#include "strapdown_ecef.h"
#include "geoparam_wgs84.h"
#include "lib_Frames.h"

#include "Trajectory.h"

using namespace std;

int main(int argc, char **argv) {

	//Set precision
	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	IMUSimulator::Trajectory traj;
	IMUSimulator::Position_IMU position;

	double temp[3] = { 6380047.0, 0.0, 0.0 };
	double wn, tow;
	wn = 1956;
	tow = 1;

	position.ecef[0] = temp[0];
	position.ecef[1] = temp[1];
	position.ecef[2] = temp[2];
	position.wn = wn;
	position.tow = tow;

	for (size_t i = 0; i < 5; i++) {

		position.ecef[2] += 1;
		position.tow += 1.0;

		//std::cout << position << std::endl;
		traj.add_position(IMUSimulator::ECEF_Frame, IMUSimulator::GPSTime, position);
	}

	std::cout << traj;


	getchar();
	return 0;
}
