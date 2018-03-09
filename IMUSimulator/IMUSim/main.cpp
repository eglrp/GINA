#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linalg.h"
#include "ConcreteDataProcess.h"
#include "Data.h"
#include "PinaFile.h"
#include "InsTkFunctions.h"

using namespace std;

int main(int argc, char **argv){
	alglib::boolean_1d_array b1;
	b1 = "[true]";

	//typedef std::numeric_limits< double > dbl;
	//cout.precision(dbl::max_digits10);

	string trajectoryFile = "example_trajectory.pina";

	PinaFile pinaFile(trajectoryFile);
	pinaFile.header.print_info();

	ConcreteDataProcess CDataProcess;
	CDataProcess.copy(pinaFile);

	geoparam_test(CDataProcess);
	getchar();
	return 0;
}