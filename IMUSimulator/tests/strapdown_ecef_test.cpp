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


	double llh_array[3] = { 0,0,0 };
	double Rn, Re, g, sL, cL, WIE_E;

	IMUSimulator::WGS84Ellipsoid myellip(llh_array, IMUSimulator::LLH_Frame);
	myellip.getParams(Rn, Re, g, sL, cL, WIE_E);

	std::cout << Rn << "   " <<
		Re << "   " <<
		g << "   " <<
		sL << "   " <<
		cL << "   " <<
		WIE_E << std::endl;

	Eigen::Vector3d acc;
	acc << 0, 0, -g;
	Eigen::Vector3d w;
	w << 0, 0, 0;

	Eigen::Vector3d ecef;
	Eigen::Vector3d llh;
	llh << 0, 0, 0;
	ecef = IMUSimulator::Lib::transform_llh2ecef(llh);
	IMUSimulator::strapdown_ecef str_e(ecef);



	for (int i = 0; i < 2; i++) {
	std:cout << endl;
		std::cout << str_e << endl;
		str_e.update(acc, w, 0.1);
	}

}