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

int main(int argc, char **argv){
	
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
