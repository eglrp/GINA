#pragma once

#include <Eigen/Dense>
#include <vector>

void get_Result(double[]);

void print_Result(void);

void get_Pos_Result(double[3]);

void set_position_of_Base(double[3]);

void set_position_of_Rover(double[3]);

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

static void setCommonSatIdandPRNvector(std::vector<int>&, std::vector<double>&, std::vector<double>&);

static void addTime2ToW_WeekRollOverChecked(int&, const double, double&, const double);

static void checkSatValidity(int, double, int[]);

static void calculateGeometryDistance(int, double, int, double[], double&, int&, double&);