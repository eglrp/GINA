
#include "CreateTrajectorywIMUGenerator.hpp"

using namespace std;


int main(int argc, char **argv) {

	std::string trajFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\60_IMUSimulator\\files\\example_trajectory_out_1ms_yaw_1.51.pina";
	std::string imuFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\60_IMUSimulator\\files\\example_imu_out_steady_state_1ms_yaw_1.51.pina";
	
	IMUGeneratorForTrajectory(trajFileNamewPath, imuFileNamewPath);

	//getchar();
	return 0;
}
