#pragma once
#include <Eigen/Dense>

namespace IMUSimulator{
	namespace Lib {

		Eigen::Vector3d transform_ecef2llh(Eigen::Vector3d&);
		void transform_ecef2llh(double&, double&, double&, double&, double&, double&);
		Eigen::Vector3d transform_llh2ecef(Eigen::Vector3d&);
		void transform_llh2ecef(double&, double&, double&, double&, double&, double&);

		Eigen::Matrix3d pos2Cne(double&, double&);
		Eigen::Matrix3d skew(Eigen::Vector3d&);
	}
}