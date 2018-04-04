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

#include "IMUStore.h"

using namespace std;

int main(int argc, char **argv) {

	//Set precision
	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	IMUSimulator::IMUStore imuStore;
	IMUSimulator::Measure_IMU meas;

	double temp_a[3] = { 0.0, 0.0, 0.0 };
	double temp_w[3] = { 0.0, 0.0, 0.0 };
	double wn, tow;
	wn = 1956;
	tow = 1.0;

	meas.a[0] = temp_a[0];
	meas.a[1] = temp_a[1];
	meas.a[2] = temp_a[2];

	meas.w[0] = temp_w[0];
	meas.w[1] = temp_w[1];
	meas.w[2] = temp_w[2];

	meas.wn = wn;
	meas.tow = tow;

	for (size_t i = 0; i < 5; i++) {

		meas.a[2] += 0.1;
		meas.tow += 1.0;

		//std::cout << position << std::endl;
		imuStore.add_measure(IMUSimulator::GPSTime, meas);
	}

	std::cout << imuStore;


	getchar();
	return 0;












}