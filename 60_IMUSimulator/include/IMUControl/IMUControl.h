#pragma once

#include <Eigen/Dense>

namespace IMUSimulator {


	class IMUControlData {

		Eigen::Vector3d ab;
		Eigen::Vector3d wb;
		double dt;

	};

}