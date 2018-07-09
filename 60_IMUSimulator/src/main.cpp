
#include "CreateTrajectorywIMUGenerator.hpp"
#include "CreateTrajectorywIMUCommand.hpp"

using namespace std;

int main(int argc, char **argv) {

	std::string trajFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\GINA\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_traj.gina";
	std::string imuFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\GINA\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_imu.gina";
	std::string imuControlFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\GINA\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_control.gina";

	//IMUGeneratorForTrajectory(trajFileNamewPath, imuFileNamewPath);
	IMUCommandForTrajectory(trajFileNamewPath, imuFileNamewPath, imuControlFileNamewPath);

	//getchar();
	return 0;
}
