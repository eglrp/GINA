
#include "CreateTrajectorywIMUGenerator.hpp"

using namespace std;


int main(int argc, char **argv) {

	std::string trajFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\30_Evaulation\\Matlab_scripts&funcs\\plots\\pinaFiles\\new_traj.pina";
	std::string imuFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\30_Evaulation\\Matlab_scripts&funcs\\plots\\pinaFiles\\new_imu.pina";
	
	IMUGeneratorForTrajectory(trajFileNamewPath, imuFileNamewPath);

	//getchar();
	return 0;
}
