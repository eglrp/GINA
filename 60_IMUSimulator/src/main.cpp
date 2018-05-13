
#include "CreateTrajectorywIMUGenerator.hpp"

using namespace std;


int main(int argc, char **argv) {

	std::string trajFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\60_IMUSimulator\\files\\example_trajectory_out_steady_state.pina";
	std::string imuFileNamewPath = "C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\60_IMUSimulator\\files\\example_imu_out_steady_state.pina";
	
	IMUGeneratorForTrajectory(trajFileNamewPath, imuFileNamewPath);

	//getchar();
	return 0;
}
