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

using namespace std;

int main(int argc, char **argv){
	
	double llh_array[3] = { 0,0,0 };
	double Rn, Re, g, sL, cL, WIE_E;

	IMUSimulator::WGS84Ellipsoid myellip(llh_array, IMUSimulator::LLH_Frame);
	myellip.getParams(Rn, Re, g, sL, cL, WIE_E);

	std::cout <<	Rn << "   " << 
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


	for (int i = 0;i<20;i++) {
		std:cout << endl;
		std::cout << str_e << endl;
		str_e.update(acc, w, 0.1);
	}
	


	/*Eigen::MatrixXd m(2, 2);
	alglib::complex_1d_array example_alglib_array();*/


	//typedef std::numeric_limits< double > dbl;
	//cout.precision(dbl::max_digits10);
	/*string trajectoryFile = "..//..//IMUSimulator//files//example_trajectory.pina";
	string IMU_inputFile = "..//..//IMUSimulator//files//example_IMU_Input.pina";

	PinaFile pinaFile(trajectoryFile);
	pinaFile.header.print_info();
	pinaFile.print_data();

	pinaFile.clear();
	pinaFile.header.print_info();
	pinaFile.print_data();

	pinaFile.open(IMU_inputFile);
	pinaFile.header.print_info();
	pinaFile.print_data();

	ConcreteDataProcess CDataProcess;
	CDataProcess.copy(pinaFile);
	CDataProcess.print();
	double foo[3] = { 0,1,3 };
	pos2cne(foo);*/
	//strap..()
	getchar();
	return 0;
}
