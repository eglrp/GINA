#include "SimpleLSSolution.h"

using namespace Eigen;
using namespace std;

static IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

static double convergenceLimitLS = 3e-3;

void simpleLS::calculateSolution(CommonTime time, vector<SatID>& prnVec, vector<double>& rangeVec, GPSEphemerisStore & bceStore)
{
	int numSats = calculatePRNsize(prnVec);
	SatID tempSat;
	double temprho;
	Triple tempSatPos;

	MatrixXd designMatrix(numSats,4);
	MatrixXd covMatrix(numSats,4);

	VectorXd y(numSats);
	VectorXd x(4);

	VectorXd prObservations = VectorXd::Zero(numSats);
	VectorXd geometricDistance = VectorXd::Zero(numSats);

	VectorXd solution = VectorXd::Zero(4);

	prObservations = stripSatSysfromObservations(prnVec, rangeVec, SatID::systemGlonass);

	// Iteration
	do {
		for (int i = 0; i < numSats;i++)	// SatID iteration
		{
			tempSat = prnVec.at(i);
			try
			{
				tempSatPos = bceStore.getXvt(tempSat, time).getPos();
			}
			catch (Exception& e)
			{
				tempSatPos = Triple(0, 0, 0);
				continue;
			}
			
			geometricDistance[i] = getGeometricDistance(solution, tempSatPos);

			for (int j = 0; j < 3;j++)			//Create Design Matrix
			{
				double aij = (solution[j] - tempSatPos[j]) / geometricDistance[i];
				designMatrix(i, j) = aij;
			}
			designMatrix(i, 3) = 1.0;
		}

		//covMatrix = getPseudoInverse(designMatrix.transpose()*designMatrix);
		covMatrix = designMatrix.transpose()*designMatrix;
		covMatrix = covMatrix.inverse();

		y = prObservations - geometricDistance;

		x = covMatrix*designMatrix.transpose()*y;

		solution[0] += x[0];
		solution[1] += x[1];
		solution[2] += x[2];
		solution[3] = x[3];


		//cout << "Iteration convergence: " << endl << getNorm(x) << endl;
		//cout << covMatrix.format(CleanFmt)<<endl;
		cout.precision(17);
		cout << "Norm: " << fixed << getNorm(x) << "  Limit:  " << fixed << convergenceLimitLS << "  Verdict: " << (getNorm(x) > convergenceLimitLS) << endl;
		cout << " Clock:   " << x[3] << endl;
	} while (getNorm(x) < convergenceLimitLS); //Todo x(1:3), clock error nem számít normába
	

	//Convergence reached
	cout << "Convergence reached: " << x << endl << "Solution: " << endl << solution[0] << "  " << solution[1] << "   " << solution[2] << endl;

}

double simpleLS::getGeometricDistance(VectorXd & solution,Triple satPos)
{
	//return sqrt(pow(solution[0] - satPos[0],2)+(solution[1] - satPos[1]) ^ 2 + (solution[2] - satPos[2]) ^ 2);
	return sqrt(pow(solution[0] - satPos[0],2)+ pow(solution[1] - satPos[1], 2)+ pow(solution[2] - satPos[2], 2));
}

void simpleLS::setConvLimit(double val)
{
	//convergenceLimitLS = val;
}
