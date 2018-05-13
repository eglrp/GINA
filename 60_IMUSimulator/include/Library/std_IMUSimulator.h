#pragma once

#include <Eigen/Dense>

namespace IMUSimulator {

	enum CoordiateFrame {
		ECEF_Frame = 0,
		LLH_Frame = 1
	};

	enum TimeFrame {
		GPSTime = 0,
		Unknown = 1
	};

	typedef struct Position_IMU_s {
		double ecef[3];
		double llh[3];
		double rollpitchyaw[3];
		unsigned int wn;
		double tow;
	}Position_IMU;

	typedef struct Measure_IMU_s {
		double a[3];
		double w[3];
		unsigned int wn;
		double tow;
	}Measure_IMU;

}