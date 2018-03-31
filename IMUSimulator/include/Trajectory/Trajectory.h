#pragma once


#include "geoparam_wgs84.h"

#include <iostream>
#include <vector>

namespace IMUSimulator {

	enum TimeFrame {
		GPSTime = 0,
		Unknown = 1
	};

	typedef struct trajectory_imu_s {
		double ecef[3];
		double llh[3];
		double wn;
		double tow;
	}Trajectory_IMU;

	class Trajectory {
		
		std::vector<Trajectory_IMU> traj_data;
		Trajectory(void);
	
		void add_position(CoordiateFrame, double[], TimeFrame, double, double);
	};



}