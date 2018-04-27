#include "TutorialConfig.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "SatID.hpp"
#include "CommonTime.hpp"
#include "Xvt.hpp"
#include "XvtStore.hpp"
#include "MatrixOperators.hpp"


#include "PRSolution.hpp"

#include "main.h"
using namespace std;
using namespace gpstk;
using gpstk::transpose;

Position rover(0.0, 0.0, 6400.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos1(6400.0, -6400.0, 22000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos2(-6400.0, 6400.0, 26000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos3(-6400.0, -6400.0, 20000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);
Position pos4(6400.0, 6400.0, 25000.0, Position::Geodetic, 0, ReferenceFrame::WGS84);

gpstk::PRSolution solution;
gpstk::Xvt xvt;
vector<Position> xvtStore;
Position basicSolution(0, 0, 0, Position::Geodetic, 0, ReferenceFrame::WGS84);


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
	int numSats = xvtStore.size();

	double x[4] = { 0,0,0,0 };
	vector<double> prObservations;
	vector<double> geometricDistance;
	Matrix<double> designMatrix(numSats + 1, numSats + 1);




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
			geometricDistance[i] = sqrt((basicSolution.X - xvtStore.at(i).X) ^ 2 + (basicSolution.Y - xvtStore.at(i).Y) ^ 2 + (basicSolution.Z - xvtStore.at(i).Z) ^ 2);
			for (int j = 0;j < 3;j++)
			{
				double rho = sqrt((basicSolution.X - xvtStore.at(i).X) ^ 2 + (basicSolution.Y - xvtStore.at(i).Y) ^ 2 + (basicSolution.Z - xvtStore.at(i).Z) ^ 2);
				double aij = (basicSolution[j] - xvtStore.at(i)[j] / rho);
				designMatrix(i, j) = aij;
			}
			designMatrix(i, 4) = 1.0;
		}

		Matrix<double> tempMat;

		tempMat = util_transpose(designMatrix);
		
		double determinant = det(tempMat);

	}





	return Position();
}

Position gpstkSolution() {

	//solution.ConvergenceLimit = convLimit;

	// Commontime, satID vector,satSys vector, psdrange vector, matrix invMC, xvtstore
	//solution.RAIMCompute(,)

	return Position();
}

template <class T, class BaseClass>
Matrix<T> util_transpose(const ConstMatrixBase<T, BaseClass>& m)
{
	Matrix<T> temp(m.cols(), m.rows());
	size_t i, j;
	for (i = 0; i < m.rows(); i++)
		for (j = 0; j < m.cols(); j++)
			temp(j, i) = m(i, j);
	return temp;
}
