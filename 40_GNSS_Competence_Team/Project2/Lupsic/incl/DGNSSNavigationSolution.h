#pragma once

#include <Eigen/Dense>

void get_Result(double[]);

void print_Result(void);

void set_time_Rover(int, double);

void set_time_Base(int, double);

void set_satId_Rover(int*, int);

void set_satId_Base(int*, int);

void set_pseudoRange_Rover(double*, int);

void set_pseudoRange_Base(double*, int);

void calculatePosition(void);

static void reset(void);

static double updatePosition(void);

static int getNumberofVisibleSatelites(int, int);

static void correctwSagnacEffect(double, double*, double*);

static double calculateDistance(double*, double*);

static double Norm(double*);

static double Norm(Eigen::VectorXd);
