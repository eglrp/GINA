
#include "CreateTrajectorywIMUGenerator.hpp"
#include "CreateTrajectorywIMUCommand.hpp"

using namespace std;

int main(int argc, char **argv) {

	std::string trajFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\GINA\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_traj.gina";
	std::string imuFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\GINA\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_imu.gina";
	
	IMUGeneratorForTrajectory(trajFileNamewPath, imuFileNamewPath);

	//getchar();
	return 0;
}
