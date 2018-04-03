#pragma once

#include "geoparam_wgs84.h"
#include "std_IMUSimulator.h"

#include <Eigen/Dense>

namespace IMUSimulator {

	class strapdown_ecef {

		public:

			double g;
			double wie_e;

			Eigen::Vector3d ecef;
			Eigen::Vector3d llh;
			WGS84Ellipsoid wgs84;
			Eigen::Matrix3d Cne;
			Eigen::Matrix3d Cbe;
			Eigen::Matrix3d Cnb;
			Eigen::Vector3d Ve;

			/*Constructor*/
			strapdown_ecef(Eigen::Vector3d&);
			strapdown_ecef(Eigen::Vector3d&, Eigen::Vector3d&);
			strapdown_ecef(Eigen::Matrix3d&, Eigen::Vector3d&, Eigen::Vector3d&);

			void update(Eigen::Matrix3d&, Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, double);
			void update(Eigen::Vector3d&, Eigen::Vector3d&, double);

			void setParams(Eigen::Matrix3d&, Eigen::Vector3d&, Eigen::Vector3d&);

			void update_gravitiy(Eigen::Vector3d);

			friend std::ostream& operator<<(std::ostream& os, const strapdown_ecef&);

		private:
			Eigen::Matrix3d strapdown_ecef::skew(Eigen::Vector3d&);
			Eigen::Vector3d strapdown_ecef::transform_ecef2llh(Eigen::Vector3d&);
			Eigen::Matrix3d pos2Cne(double&, double&);

	};
}