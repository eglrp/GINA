#include "util.h"

double getDiff(double &in1, double &in2)
{
	return abs(in1-in2);
}

double getNorm(Eigen::VectorXd  &vector)
{
	return sqrt(vector(0)*vector(0)+ vector(1)*vector(1)+ vector(2)*vector(2));
}

int calculatePRNsize(vector<gpstk::SatID>& prnvec)
{
	int result = 0;
	for (auto &it : prnvec)
	{
		if (it.system == SatID::systemGPS)
			result++;
	}
	return result;
}

Eigen::VectorXd stripSatSysfromObservations(vector<SatID>& prnvec, vector<double>& rangevec, SatID::SatelliteSystem StripSystem)
{
	//Eigen::VectorXd returnVector;
	vector<double> returnVec;
	

	for (int it = 0;it < prnvec.size();it++)
	{
		if (prnvec.at(it).system != StripSystem)
		{
			returnVec.push_back(rangevec.at(it));
		}
	}

	return Eigen::VectorXd::Map(returnVec.data(),returnVec.size());
}

void print_comparesolutions(gpstk::Vector<double> &gpstksolution, Eigen::VectorXd &lssolution)
{
	cout << "Difference between the two solutions: " << endl
		<< (gpstksolution[0] - lssolution[0]) << endl
		<< (gpstksolution[1] - lssolution[1]) << endl
		<< (gpstksolution[2] - lssolution[2]) << endl;
		/*<< gpstksolution[0] << " " << lssolution[0] << endl
		<< gpstksolution[1] << " " << lssolution[1] << endl
		<< gpstksolution[2] << " " << lssolution[2] << endl;*/
}

void getSatPos(CommonTime time)
{
	//Todo: bcestore setter and use getXvt
	
}


