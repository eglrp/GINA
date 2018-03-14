#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linalg.h"
#include "ConcreteDataProcess.h"
#include "Data.h"
#include "PinaFile.h"
#include <iostream>
using namespace std;
using namespace alglib;


double WIE_E = 7292115e-11;				//Earth rotation speed [rad/s]
double SM_AXIS = 6378137; 				//WGS84 Semi-major axis [m]
double E_SQR = 0.00669437999014;		//First eccentricity squared  1-(b^2/a^2)
double NORMAL_GRV = 9.7803253359;		//Gravity at the poles [m/s^2]
double GRV_CONS = 0.00193185265241;		//gravitation constat G =
double FLATTENING = 0.00335281066475;	// gemometry flattening (1-b/a)
double M_FAKTOR = 0.00344978650684;		//(WIE_E^2*SM_AXIS^2*b)/(G*M)

double norm(const double vector[3]) {
	double norm;
	norm = sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
	return norm;
}
double norm(const Data data) {
	double  vector[3];
	vector[0] = data.attitude[0];
	vector[1] = data.attitude[1];
	vector[2] = data.attitude[2];
	norm(vector);
}

double factorial(unsigned int n){
	unsigned long long factorial = 1;
	if (n < 0) {
		cout << "Error negative factorial!" << endl;
	}
	if (n = 0) {
		cout << "\n" << "factorial(" << n << ")=" << factorial << endl;
		return factorial;
	}
	for (int i = 1; i <= n; ++i)
	{
		factorial *= i;
	}
	cout<<"\n"<< "factorial(" << n << ")=" << factorial << endl;
	return factorial;
}

void multi_sclr(double scalar,double vector[3]) {
	int vector_length = sizeof(vector) / sizeof(double);
	for (int i = 0;i < vector_length;++i) {
		vector[i] = scalar*vector[i];
	}
}

alglib::real_2d_array pos2cne(const double position[3]) {
	double sinL = sin(position[1]);
	double cosL = cos(position[1]);
	double sinl = sin(position[0]);
	double cosl = cos(position[0]);
	double data[9] = { -sinL*cosl, -sinl, -cosL*cosl, -sinL*cosl, cosl, -cosL*sinl, cosl, 0, sinL };
	alglib::real_2d_array Cne;
	Cne.setcontent(3, 3, data);

	cout << "\n Cne.rows =" << Cne.rows() << " Cne.cols" << Cne.cols() << "\n" << endl;
	/*
	cout << Cne[0][0] << " " << Cne[0][1] << " " << Cne[0][2] << " " << endl;
	cout << Cne[1][0] << " " << Cne[1][1] << " " << Cne[1][2] << " " << endl;
	cout << Cne[2][0] << " " << Cne[2][1] << " " << Cne[2][2] << " " << endl;
	*/
	return Cne;
}

alglib::real_2d_array pos2cne(const Data data) {
	double position[3];
	position[0] = data.position[0];
	position[1] = data.position[1];
	position[2] = data.position[2];
	return pos2cne(position);
}

struct geoparam_struct {
	double radius[2];
	double gravity;
};
geoparam_struct geoparam(const double position[3]) {
	geoparam_struct parameters;
	//LLH = Latitude, Longitude, Height
	double sinL = sin(position[0]);
	double cosL = cos(position[0]);
	double h = position[2];

	double Rn = 6335439.327292829 / (sqrt(1.0 - E_SQR*sinL*sinL)*(1.0 - E_SQR*sinL*sinL));
	double Re = SM_AXIS / (sqrt(1.0 - E_SQR*sinL*sinL));
	parameters.radius[0] = Rn;
	parameters.radius[1] = Re;

	double g1 = NORMAL_GRV*(1 + GRV_CONS*pow(sinL, 2)) / (sqrt(1.0 - E_SQR*pow(sinL, 2)));
	double g = g1*(1.0 - (2.0 / SM_AXIS)*(1.0 + FLATTENING + M_FAKTOR - 2.0*FLATTENING*pow(sinL, 2))*h + 3.0*pow(h, 2) / SM_AXIS / SM_AXIS);
	parameters.gravity = g;

	return parameters;
}
geoparam_struct geoparam(const Data data) {
	double position[3];
	position[0] = data.position[0];
	position[1] = data.position[1];
	position[2] = data.position[2];

	return geoparam(position);
}

/*Just for test... 2018.03.08 - The test was succeeded!*/
void geoparam_test(ConcreteDataProcess CDP) {

	int number_of_test = CDP.data.size();
	double own_gravity;
	double pos_vec_length = sizeof(Data::position) / sizeof(double);
	ofstream own_geoparam_out_file;
	/*Write to file the position data[]*/
	own_geoparam_out_file.open("positions_data.txt");
	for (int i = 0; i < number_of_test;++i) {
		for (int j = 0;j < pos_vec_length;++j) {
			own_geoparam_out_file << CDP.data[i].position[j] << " ";
		}
		own_geoparam_out_file << endl;
	}
	own_geoparam_out_file.close();
	/*Write to file geparam.gravity*/
	own_geoparam_out_file.open("own_geoparam_result.txt");
	vector<double> own_geopram;
	for (int i = 0; i < number_of_test;++i) {
		own_gravity = geoparam(CDP.data[i]).gravity;
		own_geoparam_out_file << own_gravity << endl;
		own_geopram.push_back(own_gravity);
	}
	own_geoparam_out_file.close();
	cout << "Data were wrote to files, for the test of geoparam()!" << endl;
}


real_2d_array update_attitude( const double attitude[3], double dt) {
	
	double  rot[3];
	
	for (int i = 0; i<sizeof(rot)/sizeof(double); i++) {
		rot[i] = attitude[i];
	}
	
	multi_sclr(dt, rot);
	double rot_norm;
	rot_norm = norm(rot);
	
	double sr_a = 1 - (pow(rot_norm,2) / factorial(3)) + (pow(rot_norm, 4)/factorial(5));
	double sr_b = (1 /2) - (pow(rot_norm, 2) / factorial(4)) + (pow(rot_norm, 4) / factorial(6));

	real_2d_array eye("[1,0,0],[0,1,0],[0,0,1]");
	real_2d_array skew_rot;
	double data[9] = {0,-rot[2],rot[1] ,rot[2],0,-rot[0],-rot[1],rot[0],0};
	skew_rot.setcontent(3, 3, data);
	real_2d_array mx_a;
	real_2d_array C;
	rmatrixgemm(3, 3, 3, sr_b,skew_rot, 0, 0, 0, skew_rot, 0, 0, 0, 0, C, 0, 0);
	real_2d_array B;
	rmatrixgemm(3, 3, 3, sr_a, skew_rot, 0, 0, 0, eye, 0, 0, 0, 0, B, 0, 0);
	//mx_a = eye + B + C;
/*meg kell írni az összeadást*/
}