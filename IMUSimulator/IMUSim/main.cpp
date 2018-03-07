#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linalg.h"
#include "ConcreteDataProcess.h"
#include "Data.h"
#include "PinaFile.h"

using namespace std;

struct geoparam_struct{
	double radius[2];
	double gravity;
};

geoparam_struct goparam(Data data){
	geoparam_struct parameters;

	double WIE_E = 7292115e-11;				//Earth rotation speed [rad/s]
	double SM_AXIS = 6378137; 				//WGS84 Semi-major axis [m]
	double E_SQR = 0.00669437999014;		//First eccentricity squared  1-(b^2/a^2)
	double NORMAL_GRV = 9.7803253359;		//Gravity at the poles [m/s^2]
	double GRV_CONS = 0.00193185265241;		//gravitation constat G =
	double FLATTENING = 0.00335281066475;	// gemometry flattening (1-b/a)
	double M_FAKTOR = 0.00344978650684;		//(WIE_E^2*SM_AXIS^2*b)/(G*M)

	double sinL = sin(data.position[1]);
	double cosL = cos(data.position[1]);
	double h = data.position[3];

	double Rn = 6335439.327292829 / (sqrt(1.0 - E_SQR*sinL*sinL)*(1.0 - E_SQR*sinL*sinL));
	double Re = SM_AXIS / (sqrt(1.0 - E_SQR*sinL*sinL));
	parameters.radius[0] = Rn;
	parameters.radius[1] = Re;

	double g1 = NORMAL_GRV*(1 + GRV_CONS*sinL*sinL) / (sqrt(1.0 - E_SQR*sinL*sinL));
	double g = g1*(1.0 - (2.0 / SM_AXIS)*(1.0 + FLATTENING + M_FAKTOR - 2.0*FLATTENING*sinL*sinL)*h + 3.0*h*h / SM_AXIS / SM_AXIS);
	parameters.gravity = g;

	return parameters;
}

int main(int argc, char **argv){
	alglib::boolean_1d_array b1;
	b1 = "[true]";

	//typedef std::numeric_limits< double > dbl;
	//cout.precision(dbl::max_digits10);
	string trajectoryFile = "example_trajectory.pina";
	string IMU_inputFile = "example_IMU_Input.pina";

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
	strap..()
	getchar();
	return 0;
}