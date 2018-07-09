#pragma once

#include "std_IMUSimulator.h"

namespace IMUSimulator{
	namespace Lib {

		Eigen::Vector3d transform_ecef2llh(const Eigen::Vector3d&);
		Eigen::Vector3d transform_ecef2llh(const double[3]);
		void transform_ecef2llh(const double&, const double&, const double&, double&, double&, double&);
		Eigen::Vector3d transform_llh2ecef(const Eigen::Vector3d&);
		void transform_llh2ecef(const double&, const double&, const double&, double&, double&, double&);

		Eigen::Matrix3d pos2Cne(const double&, const double&);
		Eigen::Matrix3d skew(const Eigen::Vector3d&);

		Eigen::Vector3d dcm2euler(const Eigen::Matrix3d dcm);
		Eigen::Matrix3d euler2dcm(const Eigen::Vector3d);
		Eigen::Matrix3d euler2dcm(const double[3]);

	}
}