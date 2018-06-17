
#include "DGNSSNavigationSolution.h"
#include "utility.h"



#define MAXSATNUMBER 15
#define CONVERGENCE_LIMIT 1E-5

using namespace Eigen;

static int WNRover, WNBase;
static double ToWRover, ToWBase;
static int satIdRover[MAXSATNUMBER] = { -1 };
static int satIdBase[MAXSATNUMBER] = { -1 };
static double prRover[MAXSATNUMBER] = { -1 };
static double prBase[MAXSATNUMBER] = { -1 };
static int vectorSizeRover = 0;
static int vectorSizeBase = 0;

static double basePosition[3];

static const double wie_e = 7.292115e-5;
static const double c_mps = 299792458;


static double roverPos[4] = {0, 0, 0, 0};
//static Matrix3d designMatrix;

void get_Result(double sol[]) {

	sol[0] = roverPos[0];
	sol[1] = roverPos[1];
	sol[2] = roverPos[2];
	sol[3] = roverPos[3];
}

void print_Result(void) {

	cout << "" << roverPos[0] << " ";
	cout << " " << roverPos[1] << " ";
	cout << " " << roverPos[2] << "	";
	cout << "Clock bias: " << roverPos[3] << endl;
}

void get_Pos_Result(double sol[3]) {

	sol[0] = roverPos[0];
	sol[1] = roverPos[1];
	sol[2] = roverPos[2];
}

void set_position_of_Base(double pos[3]) {

	basePosition[0] = pos[0];
	basePosition[1] = pos[1];
	basePosition[2] = pos[2];
}

void set_position_of_Rover(double pos[3]) {

	roverPos[0] = pos[0];
	roverPos[1] = pos[1];
	roverPos[2] = pos[2];

	/*roverPos[0] = -2500946.0962;
	roverPos[1] = -4670472.9515;
	roverPos[2] = 3539500.5469;*/
		
}

void set_time_Rover(int wn, double tow) {
	WNRover = wn;
	ToWRover = tow;
}

void set_time_Base(int wn, double tow) {
	WNBase = wn;
	ToWBase = tow;
}

void set_satId_Rover(int* id, int size) {

	for (int i = 0; i < size; i++) {

		if ( i >= MAXSATNUMBER) {
			break;
		}

		satIdRover[i] = id[i];
	}

	vectorSizeRover = size;
}

void set_satId_Base(int* id, int size) {

	for (int i = 0; i < size; i++) {

		if (i >= MAXSATNUMBER) {
			break;
		}

		satIdBase[i] = id[i];
	}

	vectorSizeBase = size;
}

void set_pseudoRange_Rover(double* psoudoRange, int size) {

	for (int i = 0; i < size; i++) {

		if (i >= MAXSATNUMBER) {
			break;
		}

		prRover[i] = psoudoRange[i];
	}

	vectorSizeRover = size;
}

void set_pseudoRange_Base(double* psoudoRange, int size) {

	for (int i = 0; i < size; i++) {

		if (i >= MAXSATNUMBER) {
			break;
		}

		prBase[i] = psoudoRange[i];
	}

	vectorSizeBase = size;
}

void calculatePosition(void) {

	while (updatePosition() > CONVERGENCE_LIMIT);
	reset();
}

static double updatePosition(void) {

	//vectorSize = getNumberofVisibleSatelites(WN, ToW);
	double rhoRover, rhoBase;
	double temp_satPos[3];
	
	vector<int> SatIdv;
	vector<double> prvRover; 
	vector<double> prvBase;

	setCommonSatIdandPRNvector(SatIdv, prvRover, prvBase);

	MatrixXd covMatrix = MatrixXd::Zero(4, 4);
	MatrixXd designMatrix(SatIdv.size(), 4);
	VectorXd geometryDistanceBase = VectorXd::Zero(SatIdv.size());
	VectorXd geometryDistanceRover = VectorXd::Zero(SatIdv.size());
	VectorXd PRObservationsBase = VectorXd::Zero(SatIdv.size());
	VectorXd PRObservationsRover = VectorXd::Zero(SatIdv.size());
	VectorXd y(SatIdv.size());
	VectorXd x(4);

	for (int i = 0; i < SatIdv.size(); i++) {
		
		/*if (satIdRover[i] < 0) {
			continue;
		}*/

		// Get i. satelite position at given time
		if (get_satPos(WNRover, ToWRover, SatIdv[i], temp_satPos) == 0) {
			throw("After satellite sanity check this error shall never happen. \n WNRover: %lf \n ToWRover: %lf \n SatIdv: %d", WNRover, ToWRover, SatIdv[i]);
		}

		PRObservationsRover(i) = prvRover[i];
		PRObservationsBase(i) = prvBase[i];

		int WN_Rover_TransmissionTime, WN_Base_TransmissionTime;
		double ToW_Rover_TransmissionTime, ToW_Base_TransmissionTime;

		// Calculate Geometry Distance and Transmission Time
		calculateGeometryDistance(WNRover, ToWRover, SatIdv[i], roverPos, rhoRover, WN_Rover_TransmissionTime, ToW_Rover_TransmissionTime);
		calculateGeometryDistance(WNBase, ToWBase, SatIdv[i], basePosition, rhoBase, WN_Base_TransmissionTime, ToW_Base_TransmissionTime);
		
		geometryDistanceRover(i) = rhoRover;
		geometryDistanceBase(i) = rhoBase;

		// Set up design matrix
		designMatrix(i, 0) = (roverPos[0] - temp_satPos[0]) / geometryDistanceRover(i);
		designMatrix(i, 1) = (roverPos[1] - temp_satPos[1]) / geometryDistanceRover(i);
		designMatrix(i, 2) = (roverPos[2] - temp_satPos[2]) / geometryDistanceRover(i);
		designMatrix(i, 3) = 1.0;
	}

	// Create covariance matrix
	covMatrix = designMatrix.transpose() * designMatrix;
	covMatrix = covMatrix.inverse();

	// Set up observetion vector 
	// Multiply Covariance matrix with A.' and PR residual matrix
	y = PRObservationsRover - PRObservationsBase - geometryDistanceRover + geometryDistanceBase;
	x = covMatrix * designMatrix.transpose() * y;

	// Update rover position and clock bias
	roverPos[0] = roverPos[0] + x(0);
	roverPos[1] = roverPos[1] + x(1);
	roverPos[2] = roverPos[2] + x(2);
	roverPos[3] = x(3);

	double res_norm = Norm(x);
	return res_norm;
}

static void reset(void) {

	for (int i = 0; i < MAXSATNUMBER; i++) {
		satIdRover[i] = 0;
		prRover[i] = 0;
		vectorSizeRover = 0;

		satIdBase[i] = 0;
		prBase[i] = 0;
		vectorSizeBase = 0;
	}
}

static int getNumberofVisibleSatelites(int WN, int ToW) {

	double temp_satPos[3];

	int numberofSats = 0;
	for (int Id = 0; Id < 33; Id++){
		if (get_satPos(WN, ToW, Id, temp_satPos)) {
			numberofSats++;
		}
	}

	return numberofSats;
}

static void correctwSagnacEffect(double deltat, double* old_pos, double* new_pos) {

	Matrix3d rotMatrix = Matrix3d::Zero();

	rotMatrix <<	cos(wie_e*deltat),  -sin(wie_e*deltat),		0,
					sin(wie_e*deltat),	 cos(wie_e*deltat),		0,
					0,					 0,						1;

	Vector3d posV;
	posV << old_pos[0], old_pos[1], old_pos[2];

	posV = rotMatrix*posV;

	new_pos[0] = posV(0);
	new_pos[1] = posV(1);
	new_pos[2] = posV(2);

}

static double calculateDistance(double* rover, double* sat) {

	double dist = 0;
	for (int a = 0; a < 3; a++) {
		dist += pow((rover[a] - sat[a]), 2);
	}

	return sqrt(dist);
}

static double Norm(double* x) {

	return sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
}

static double Norm(VectorXd x) {

	return sqrt(x(0) * x(0) + x(1) * x(1) + x(2) * x(2));
}

static void setCommonSatIdandPRNvector(vector<int> &SatIdv, vector<double> &prvRover, vector<double> &prvBase) {

	double temp_satPos[3];
	double tempToW;
	int tempWN;

	checkSatValidity(WNRover, ToWRover, satIdRover);
	checkSatValidity(WNBase, ToWBase, satIdBase);
	
	for (size_t itRover = 0; itRover < vectorSizeRover; itRover++){
		for (size_t itBase = 0; itBase < vectorSizeBase; itBase++){
			if (satIdRover[itRover] == satIdBase[itBase] && satIdRover[itRover] > 0) {
				SatIdv.push_back(satIdRover[itRover]);
				prvRover.push_back(prRover[itRover]);
				prvBase.push_back(prBase[itBase]);
			}
		}
	}

}

static void addTime2ToW_WeekRollOverChecked(int &WN, const double t_in, double &t_out, const double deltaT) {
	
	double week = 604800;
	
	if (deltaT > week || deltaT < -week) {
		throw("In checkForGpsWeekRollOver the delta time is greater than 604800 or less than -604800. The function cannot handle this case. This function shall be updated to handle this case.");
	}

	t_out = t_in + deltaT;

	if (t_out > week) {
		t_out -= week;
		WN += 1;
	}

	if (t_out < 0){
		t_out +=  week;
		WN -= 1;
	}

}

static void checkSatValidity(int WN, double ToW, int satId[]) {

	double temp_satPos[3];
	double tempToW;
	int tempWN;
	int vectorSize;
	double maxTransmissionTime = 0.086;

	vectorSize = sizeof(satId) / sizeof(int);

	for (size_t i = 0; i < vectorSize; i++) {

		if (satId[i] < 1) {
			continue;
		}

		// Get i. satelite position at given time
		tempToW = ToW ;
		tempWN = WN;
		addTime2ToW_WeekRollOverChecked(tempWN, ToW, tempToW, -maxTransmissionTime);

		if (get_satPos(tempWN, tempToW, satId[i], temp_satPos) == 0) {
			satId[i] = 0;
		}
	}

}

static void calculateGeometryDistance(int WN, double ToW, int SatId, double Pos[], double &geometryDistance, int &WN_TransmissionTime, double &ToW_TransmissionTime) {

	double rho = 0;
	double temp_satPos[3], temp_iter_satPos[3];
	double temp_satClock, temp_satRelCorr;
	double time_of_transmission, time_of_arrival;
	double travelTime = 0.0, travelTime_old = 0.0;
	double temp_dist;

	// Get i. satelite position at given time
	if (get_satPos(WN, ToW, SatId, temp_satPos) == 0) {
		throw("After satellite sanity check this error shall never happen. \n WNRover: %lf \n ToWRover: %lf \n SatId: %d", WN, ToW, SatId);
	}

	//PRObservations(i) = prRover[i];

	// Calculate geometry distance from i. svh
	// Calculate normal vector to i. svh
	rho = 0;
	geometryDistance = 0;

	// Correct with earth roation
	temp_iter_satPos[0] = temp_satPos[0];
	temp_iter_satPos[1] = temp_satPos[1];
	temp_iter_satPos[2] = temp_satPos[2];

	time_of_arrival = ToW;
	time_of_transmission = ToW;
	WN_TransmissionTime = WN;
	
	temp_dist = calculateDistance(Pos, temp_iter_satPos);

	travelTime_old = travelTime;
	travelTime = temp_dist / c_mps;

	get_satPos(WN, time_of_arrival, SatId, temp_satPos);

	addTime2ToW_WeekRollOverChecked(WN_TransmissionTime, time_of_arrival, time_of_transmission, -travelTime);	//time_of_transmission = time_of_arrival - travelTime;
	
	get_satPos(WN_TransmissionTime, time_of_transmission, SatId, temp_satPos);

	correctwSagnacEffect(time_of_transmission - time_of_arrival, temp_satPos, temp_iter_satPos);

	temp_satPos[0] = temp_iter_satPos[0];
	temp_satPos[1] = temp_iter_satPos[1];
	temp_satPos[2] = temp_iter_satPos[2];

	// Geometry distance
	geometryDistance = calculateDistance(Pos, temp_satPos);
	ToW_TransmissionTime = time_of_transmission;
}