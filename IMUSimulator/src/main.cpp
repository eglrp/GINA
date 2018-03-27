#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linalg.h"
#include "ConcreteDataProcess.h"
#include "Data.h"
#include "PinaFile.h"
#include "GPSWeekSecond.hpp"
#include "dummy.h"
#include "InsTkFunctions.h"
using namespace std;

int main(int argc, char **argv){
	
	alglib::complex_1d_array example_alglib_array();


	//typedef std::numeric_limits< double > dbl;
	// cout.precision(dbl::max_digits10);
	string trajectoryFile = "..//..//IMUSimulator//files//example_trajectory.pina";
	string IMU_inputFile = "..//..//IMUSimulator//files//steady_state_IMU_Input.pina";
/*
	PinaFile pinaFile(trajectoryFile);
	pinaFile.header.print_info();
	pinaFile.print_data();

	pinaFile.clear();
	pinaFile.header.print_info();
	pinaFile.print_data();
*/
	PinaFile pinaFile;
	pinaFile.open(IMU_inputFile);
	pinaFile.header.print_info();
	pinaFile.print_data();

	ConcreteDataProcess CDataProcess;
	CDataProcess.copy(pinaFile);
	CDataProcess.print_start_values();
	CDataProcess.print_data();

	/*
	double foo_to_cne[3] = { 0,1,3 };
	pos2cne(foo_to_cne);
	//In this case, cbn = eye; and I'v got eye -> pos2cbn() OK! 2018.03.21.
	double foo_to_cbe[3] = { 0,0,0 }; 
	pos2cbn(foo_to_cbe);
	//strap..()

	Cbe(pos2cne(foo_to_cne), pos2cbn(foo_to_cbe));
	getchar();
	*/
	strapdown(CDataProcess);
	getchar();

	return 0;
}
