#pragma once

#include "std_IMUSimulator.h"
#include "geoparam_wgs84.h"
#include "lib_Frames.h"

#include <iostream>
#include <vector>

namespace IMUSimulator {

	class IMUSignalGenerator {

		public:

			double g;
			double wie_e;
			Measure_IMU meas;

			WGS84Ellipsoid wgs84;

			IMUSignalGenerator(void);
			void addError(void);
			Measure_IMU calculate(Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d);

		private:

			void IMUSignalGenerator::update_gravitiy(double, double, double);
			Eigen::Matrix3d IMUSignalGenerator::pos2Cne_RAD(double& lat, double& lon);

	};






}