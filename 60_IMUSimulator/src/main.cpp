
#include "GINAConfig.h"

#include "CreateTrajectorywIMUGenerator.hpp"
#include "CreateTrajectorywIMUCommand.hpp"

using namespace std;

int main(int argc, char **argv) {

	std::string trajFileNamewPath = ROOT "\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_traj.gina";
	std::string imuFileNamewPath = ROOT "\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_imu.gina";
	std::string imuControlFileNamewPath = ROOT "\\30_Evaulation\\Matlab_scripts&funcs\\plots\\ginaFiles\\new_control.gina";

	//IMUGeneratorForTrajectory(trajFileNamewPath, imuFileNamewPath);
	IMUCommandForTrajectory(trajFileNamewPath, imuFileNamewPath, imuControlFileNamewPath);

	//getchar();
	return 0;
}
