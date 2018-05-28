#pragma once

#include "std_IMUSimulator.h"

namespace IMUSimulator{
	namespace Lib {

		const Eigen::Vector3d transform_ecef2llh(Eigen::Vector3d&);
		void transform_ecef2llh(double&, double&, double&, double&, double&, double&);
		Eigen::Vector3d transform_llh2ecef(Eigen::Vector3d&);
		void transform_llh2ecef(double&, double&, double&, double&, double&, double&);

		Eigen::Matrix3d pos2Cne(double&, double&);
		Eigen::Matrix3d skew(Eigen::Vector3d&);

		Eigen::Vector3d dcm2euler(Eigen::Matrix3d dcm);
		Eigen::Matrix3d euler2dcm(Eigen::Vector3d);

	}
}