#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linalg.h"
#include "ConcreteDataProcess.h"
#include "Data.h"
#include "PinaFile.h"
#include "GPSWeekSecond.hpp"
#include "InsTkFunctions.h"

#include <Eigen/Dense>

#include "strapdown_ecef.h"
#include "geoparam_wgs84.h"
#include "lib_Frames.h"

#include "Trajectory.h"
#include "IMUSignalGenerator.h"
#include "IMUStore.h"

using namespace std;

int main(int argc, char **argv){
	
	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	double llh_array[3] = { 0,0,0 };
	double Rn, Re, g, sL, cL, WIE_E;

	IMUSimulator::IMUSignalGenerator imuGenerator;

	IMUSimulator::WGS84Ellipsoid myellip(llh_array, IMUSimulator::LLH_Frame);
	myellip.getParams(Rn, Re, g, sL, cL, WIE_E);

	std::cout	<<	Rn	<< "   " <<
					Re	<< "   " <<
					g	<< "   " <<
					sL	<< "   " <<
					cL	<< "   " <<
					WIE_E << std::endl;

	Eigen::Vector3d ab, wb, Vb, ab_comp, wb_comp, Vb_comp;
	ab << 0, 0, 0;
	Eigen::Vector3d bode2ecef_angle, nav2ecef_angle, local_angle;
	wb << 0, 0, 0;

	Eigen::Matrix3d Cnb;
	Eigen::Vector3d ecef;
	Eigen::Vector3d llh;
	llh << 0, 0, 0;
	ecef = IMUSimulator::Lib::transform_llh2ecef(llh);
	IMUSimulator::strapdown_ecef str_e(ecef);
	IMUSimulator::IMUStore imuStore;
	IMUSimulator::Trajectory traj;

	IMUSimulator::Measure_IMU meas;

	double dt = 0.1;
	double time = 0.0;
	double time_max = 1.0;

	for (time = 0; time < time_max; time+=dt) {

		std:cout << endl;
		
		/*nav2ecef_angle = IMUSimulator::Lib::dcm2euler(str_e.Cne);
		std::cout << "Roll, pitch, yaw: " << nav2ecef_angle << std::endl;
		bode2ecef_angle = IMUSimulator::Lib::dcm2euler(str_e.Cbe);
		std::cout << "Roll, pitch, yaw: " << bode2ecef_angle << std::endl;*/

		Cnb = str_e.Cne.transpose()*str_e.Cbe;
		local_angle = IMUSimulator::Lib::dcm2euler(Cnb);

		Vb = str_e.getVbody();
		llh = str_e.getLLH();

		/*std::cout << "Acc body: " << std::endl << ab << std::endl;
		std::cout << "Angular rate body: " << std::endl << wb << std::endl;
		std::cout << "Velocity body: " << std::endl << Vb << std::endl;
		std::cout << "Lat lon height: " << std::endl << llh << std::endl;
		std::cout << "Roll, pitch, yaw: " << std::endl << local_angle << std::endl;*/

		meas = imuGenerator.calculate(ab, wb, Vb, llh, local_angle);

		meas.wn = 1956;
		meas.tow = time;

		//std::cout << "IMU measurement: " << std::endl << meas << std::endl;

		imuStore.add_measure(IMUSimulator::GPSTime, meas);

		str_e.update(meas, dt);
		std::cout << str_e << endl;
		traj << str_e;
	}

	//std::cout << imuStore << endl;
	std::cout << traj << endl;

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
