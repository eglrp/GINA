#pragma once

#include <math.h>
#include <Eigen\Dense>
#include <Eigen\Core>

#include <vector>

#include "CommonTime.hpp"
#include "SatID.hpp"
#include "GPSEphemerisStore.hpp"
#include "GNSSconstants.hpp"

using namespace gpstk;
using namespace std;

static GPSEphemerisStore bceStore;
static const double C_MPS = 2.99792458e8;

//---------------------------------------\\

void setStore(GPSEphemerisStore & in);
GPSEphemerisStore getStore();

double getDiff(double &in1, double &in2);

double getNorm(Eigen::VectorXd &vector);

int calculatePRNsize(vector<gpstk::SatID> &prnvec);

Eigen::VectorXd stripSatSysfromObservations(vector<SatID>&prnvec, vector<double>&rangevec, SatID::SatelliteSystem StripSystem);

void print_comparesolutions(gpstk::Vector<double> &gpstksolution, Eigen::VectorXd &lssolution);

void getSatPos(CommonTime time);

double getSignalTravelTime(double psdrange);