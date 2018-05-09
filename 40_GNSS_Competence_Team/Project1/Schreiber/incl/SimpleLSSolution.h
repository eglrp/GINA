#pragma once

#include "util.h"

using namespace gpstk;
using namespace std;
using namespace Eigen;


namespace simpleLS {

	

	void calculateSolution(CommonTime time,vector<SatID> &prnVec,vector<double> &rangeVec,GPSEphemerisStore &bceStore);
	
	double getGeometricDistance(VectorXd &solution,Triple satPos);

	void setConvLimit(double val);

}