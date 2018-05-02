#pragma once

#include <math.h>
#include <Eigen\Dense>
#include <Eigen\Core>

#include <vector>

#include "PRSolution2.hpp"
#include "CommonTime.hpp"
#include "SatID.hpp"
#include "GPSEphemerisStore.hpp"
#include "GNSSconstants.hpp"

using namespace gpstk;
using namespace std;

double getDiff(double &in1, double &in2);

double getNorm(Eigen::VectorXd &vector);

int calculatePRNsize(vector<gpstk::SatID> &prnvec);

Eigen::VectorXd stripSatSysfromObservations(vector<SatID>&prnvec, vector<double>&rangevec, SatID::SatelliteSystem StripSystem);
