#include "TutorialConfig.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "linalg.h"
#include "alglibinternal.h"

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "SatID.hpp"
#include "CommonTime.hpp"
#include "Xvt.hpp"
#include "XvtStore.hpp"

#include "PRSolution.hpp"

#include <Eigen/Dense>
using namespace std;
using namespace gpstk;

Position rover(0.0, 0.0, 6400.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos1(6400.0, -6400.0, 22000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos2(-6400.0, 6400.0, 26000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos3(-6400.0, -6400.0, 20000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos4(6400.0, 6400.0, 25000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);

gpstk::PRSolution solution;
gpstk::Xvt xvt;
vector<Position> xvtStore;
Xvt basicSolution;

int main(int argc, char **argv) {
	
	xvtStore.clear();

	xvtStore.push_back(pos1);
	xvtStore.push_back(pos2);
	xvtStore.push_back(pos3);
	xvtStore.push_back(pos4);

	basicLSQSolution();
	
	return 0;
}


Position basicLSQSolution() {

	float convLimit = 3e-3;

	double x[4] = { 0,0,0,0 };
	vector<double> prObservations;
	vector<double> geometricDistance;
	

	int numSats = xvtStore.size();

	for (int i = 0; i < numSats;i++)
	{
		// Initial Geometric Distance
		prObservations[i] = sqrt(((rover[0] - xvtStore.at(i).X) ^ 2 + (rover[1] - xvtStore.at(i).Y) ^ 2 + (rover[2] - xvtStore.at(i).Z) ^ 2));

		//Additional Clock Bias Errors
		prObservations[i] += 100.0;
	}

	for (int i = 0;i < 2000;i++)
	{
		for (int i = 0;i < numSats;i++)
		{
			//geometricDistance[i] = sqrt()
		}
	}





	return Position();
}

Position gpstkSolution() {

	//solution.ConvergenceLimit = convLimit;

	// Commontime, satID vector,satSys vector, psdrange vector, matrix invMC, xvtstore
	//solution.RAIMCompute(,)

	return Position();
}

