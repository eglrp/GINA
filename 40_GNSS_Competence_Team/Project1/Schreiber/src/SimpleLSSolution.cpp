#include "SimpleLSSolution.h"

using namespace Eigen;
using namespace std;

static IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
static vector<double> extgpstksol;
static VectorXd solution;

static double convergenceLimitLS = 3e-3;


void simpleLS::calculateSolution(CommonTime time, vector<SatID>& prnVec, vector<double>& rangeVec, GPSEphemerisStore & bceStore)
{
	int numSats = calculatePRNsize(prnVec);
	SatID tempSat;
	double temprho;
	double temp_clockbias, temp_satRelCorr;
	Triple tempSatPos,temp_OldSatPos;


	MatrixXd designMatrix(numSats,4);
	MatrixXd covMatrix(numSats,4);

	VectorXd y(numSats);
	VectorXd x(4);

	VectorXd prObservations = VectorXd::Zero(numSats);
	VectorXd geometricDistance = VectorXd::Zero(numSats);
	double temp_distance, sigTTime = 0.0, sigTTime_old = 0.0;
	GPSWeekSecond timeOfTransmission;
	double tempTime;
	int WN;

	solution = VectorXd::Zero(4);

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
			// Iterate satellite position with the signal travel time
			timeOfTransmission = ((GPSWeekSecond)time);
			temp_OldSatPos = tempSatPos;

			for (int it = 0; it < 5;it++) {
				temp_distance = getGeometricDistance(solution, tempSatPos);
				
				sigTTime_old = sigTTime;
				sigTTime = temp_distance / C_MPS;

				tempSatPos = bceStore.getXvt(tempSat, timeOfTransmission).getPos();

				timeOfTransmission.sow = ((GPSWeekSecond)time).sow - sigTTime;
				tempSatPos = bceStore.getXvt(tempSat, timeOfTransmission).getPos();

			}
			tempTime = (CommonTime)timeOfTransmission - time;
			corrW_Sagnac(tempTime, &tempSatPos);

			geometricDistance[i] = getGeometricDistance(solution, tempSatPos);

			temp_clockbias = getClockBias(tempSat, timeOfTransmission);
			temp_satRelCorr = getSatRelCorr(tempSat, timeOfTransmission);
			prObservations(i) += (temp_clockbias + temp_satRelCorr) * C_MPS;

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
		/*cout.precision(17);
		cout << "Norm: " << fixed << getNorm(x) << "  Limit:  " << fixed << convergenceLimitLS << "  Verdict: " << (getNorm(x) > convergenceLimitLS) << endl;
		cout << " Clock:   " << x[3] << endl;*/
	} while (getNorm(x) < convergenceLimitLS); //Todo x(1:3), clock error nem számít normába
	

	//Convergence reached
	//cout << "Convergence reached: " << x << endl << "Solution: " << endl << solution[0] << "  " << solution[1] << "   " << solution[2] << endl;

}

double simpleLS::getGeometricDistance(VectorXd & solution,Triple satPos)
{
	//return sqrt(pow(solution[0] - satPos[0],2)+(solution[1] - satPos[1]) ^ 2 + (solution[2] - satPos[2]) ^ 2);
	return sqrt(pow(solution[0] - satPos[0],2)+ pow(solution[1] - satPos[1], 2)+ pow(solution[2] - satPos[2], 2));
}

VectorXd simpleLS::getSolution()
{
	return solution;
}

void simpleLS::setConvLimit(double val)
{
	//convergenceLimitLS = val;
}

void simpleLS::corrW_Sagnac(double rho, Triple* PVT)
{
	double wt;
	Triple svxyz[3];
	GPSEllipsoid ell;
	

	wt = ell.angVelocity()*rho;             // radians
	svxyz[0] = ::cos(wt)*PVT[0] + ::sin(wt)*PVT[1];
	svxyz[1] = -::sin(wt)*PVT[0] + ::cos(wt)*PVT[1];
	svxyz[2] = PVT[2];

	PVT[0] = svxyz[0];
	PVT[1] = svxyz[1];
	PVT[2] = svxyz[2];
}

void simpleLS::calcCorrections(VectorXd & prvec,vector<SatID> prn, CommonTime time)
{
	Xvt satXvt;
	CommonTime Tr = time;
	double deltaT = 0;

	for (int i = 0; i < prvec.size();i++)
	{
		Tr = time;
		for (int j = 0; j < 5;j++) {

			Tr -= getSignalTravelTime(prvec[i]);
			satXvt = getSatXVT(Tr, prn.at(i));
			//corrW_Sagnac(prvec.data[i]);
		}
	}
}

Xvt simpleLS::getSatXVT(CommonTime time, SatID sat)
{
	return getStore().getXvt(sat,time);
}

double simpleLS::getClockBias(SatID id, CommonTime time)
{
	double clockBias;

	Xvt satXVT;

	try
	{
		satXVT = getSatXVT(time, id);
		clockBias = satXVT.getClockBias();
	}
	catch (const std::exception&)
	{
		return 0;
	}
	return clockBias;
}

double simpleLS::getSatRelCorr(SatID id, CommonTime time)
{
	double relcorr;

	Xvt satXVT;

	try
	{
		satXVT = getSatXVT(time, id);
		relcorr = satXVT.getRelativityCorr();
	}
	catch (const std::exception&)
	{
		return 0;
	}
	return relcorr;
}
