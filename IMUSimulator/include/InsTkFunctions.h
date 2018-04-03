#pragma once  // midnen headerbe tedd bele! 
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linalg.h"
#include "ConcreteDataProcess.h"
#include "Data.h"
#include "PinaFile.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace alglib;

/*Basic matematicall functions*/
double norm(const double vector[3]);
double norm(const Data data);
double factorial(const int n);
double* add(const double a[], const double b[]);

real_2d_array add(const real_2d_array A, const real_2d_array B);
real_2d_array add(const real_2d_array A, const real_2d_array B, const real_2d_array C);

double* cross(double a[], double b[]);
double* matrix_vector_product(real_2d_array M, double* v);
double* multiply_sclr(const double scalar, const double vector[3]);

double* skew_data(double* rot);

/*Coordinate transformaton functions*/
real_2d_array pos2cne(const double position[3]);
real_2d_array pos2cne(const Data data);
real_2d_array pos2cbn(const double attitude[3]);
real_2d_array pos2cbn(const Data IMU_data);
real_2d_array Cbe(const real_2d_array Cne, const real_2d_array Cbn);

/*Main steps for strapdown algorithm*/
struct geoparam_struct;
geoparam_struct geoparam(const double position[3]);
geoparam_struct geoparam(const Data data);
real_2d_array update_attitude(const double angular_velocity[3], const double dt, const real_2d_array C_be);
void update_velocity(double velocity[3], const double acceleration[3], const double dt, const real_2d_array C_be, const double ge[3], double Ve_new[3]);
double* update_position(const double* velocity, const double* position, double dt);
void attitude_from_matrix(const real_2d_array C_nb, double attitude[3],string type);

/*Strapdown function*/
ConcreteDataProcess strapdown(ConcreteDataProcess CDP_IMU_Data);

/*Just for test. 2018.03.08 - The test was succeeded!*/
void geoparam_test(ConcreteDataProcess CDP);