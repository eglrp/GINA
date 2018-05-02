#pragma once

#include <Eigen/Dense>

void get_Result(double[]);

void print_Result(void);

void set_time(int, double);

void set_satId(int*, int);

void set_pseudoRange(double*, int);

void calculatePosition(void);

static void reset(void);

static double updatePosition(void);

static int getNumberofVisibleSatelites(int, int);

static void correctwSagnacEffect(double, double*, double*);

static double calculateDistance(double*, double*);

static double Norm(double*);

static double Norm(Eigen::VectorXd);
