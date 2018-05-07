
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Own includes
#include "TrajectoryReader.hpp"
#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"

#include "CoordinateFrames.h"
#include "CoordinateFrameHandler.h"

using namespace std;

int main(int argc, char **argv) {

	gnsssimulator::TrajectoryStream trajFileIn("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\PINAParser\\files\\example_trajectory.pina");
	gnsssimulator::TrajectoryStream trajFileOut("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\PINAParser\\files\\example_trajectory.pina_out.pina", std::ios::out);
	gnsssimulator::TrajectoryHeader trajHeader;

	trajFileIn >> trajHeader;
	trajFileOut << trajHeader;

	trajFileIn.close();
	trajFileOut.close();

//typedef std::numeric_limits< double > dbl;
//cout.precision(dbl::max_digits10);
/*string trajectoryFile = "..//..//IMUSimulator//files//example_trajectory.pina";
string IMU_inputFile = "..//..//IMUSimulator//files//example_IMU_Input.pina";

PinaFile pinaFile(trajectoryFile);
pinaFile.header.print_info();
pinaFile.print_data();

pinaFile.clear();
pinaFile.header.print_info();
pinaFile.print_data();

pinaFile.open(IMU_inputFile);
pinaFile.header.print_info();
pinaFile.print_data();

ConcreteDataProcess CDataProcess;
CDataProcess.copy(pinaFile);
CDataProcess.print();
double foo[3] = { 0,1,3 };
pos2cne(foo);*/
//strap..()

getchar();
return 0;

}
