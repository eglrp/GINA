#pragma once
#include <Eigen/Dense>

namespace IMUSimulator{
	namespace Lib {

		Eigen::Vector3d transform_ecef2llh(Eigen::Vector3d&);
		Eigen::Vector3d transform_llh2ecef(Eigen::Vector3d&);

		Eigen::Matrix3d pos2Cne(double&, double&);
		Eigen::Matrix3d skew(Eigen::Vector3d&);
	}
}