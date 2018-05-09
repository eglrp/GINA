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

int main(int argc, char **argv) {

	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	double llh_array[3] = { 0,0,0 };
	double Rn, Re, g, sL, cL, WIE_E;

	IMUSimulator::IMUSignalGenerator imuGenerator;

	IMUSimulator::WGS84Ellipsoid myellip(llh_array, IMUSimulator::LLH_Frame);
	myellip.getParams(Rn, Re, g, sL, cL, WIE_E);

	std::cout << Rn << "   " <<
		Re << "   " <<
		g << "   " <<
		sL << "   " <<
		cL << "   " <<
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
	double time_max = 10.0;

	for (time = 0; time < time_max; time += dt) {

		std:cout << endl;

		Cnb = str_e.Cne.transpose()*str_e.Cbe;
		local_angle = IMUSimulator::Lib::dcm2euler(Cnb);

		Vb = str_e.getVbody();
		llh = str_e.getLLH();

		/*Calculate what acc and ang vel value should be added to the dummy acc and w inputs. Dummy means without g, wie_e, and cetripetal force compenstation*/
		meas = imuGenerator.calculate(ab, wb, Vb, llh, local_angle);

		meas.wn = 1956;
		meas.tow = time;

		std::cout << "IMU measurement: " << std::endl << meas << std::endl;			// Print out the actual IMU measurement

		imuStore.add_measure(IMUSimulator::GPSTime, meas);

		str_e.update(meas, dt);
		std::cout << str_e << endl;													// Print out the actual data stored in the strapdown_ecef object
		traj << str_e;
		traj.updateTime(IMUSimulator::GPSTime, meas.wn, meas.tow);
	}

	

	std::cout << imuStore << endl;													// Print out all the IMU signal element
	std::cout << traj << endl;														//  Print out all the position element

	IMUSimulator::Position_IMU last_traj_elements = traj.traj_data.back();
	std::cout << last_traj_elements;												// Print out just the last position element

	getchar();
	return 0;
}
