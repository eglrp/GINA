#pragma once

#include "geoparam_wgs84.h"
#include "std_IMUSimulator.h"

#include "PositionData.h"

#include <Eigen/Dense>

namespace IMUSimulator {

	class strapdown_ecef {

		public:

			unsigned int GPSWeek;
			double GPSToW;
			double g;
			double wie_e;

			Eigen::Vector3d ecef;
			Eigen::Vector3d rollpitchyaw;
			WGS84Ellipsoid wgs84;
			Eigen::Vector3d Ve;

			/*Constructor*/
			strapdown_ecef(Eigen::Vector3d&);
			strapdown_ecef(Eigen::Vector3d&, Eigen::Vector3d&);
			strapdown_ecef(Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&);

			void update(Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, double);
			void strapdown_ecef::update(Measure_IMU&, double);
			void update(Eigen::Vector3d&, Eigen::Vector3d&, double);

			void setParams(Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&);
			void setTime(unsigned int&, double&);

			Eigen::Vector3d const getLLH_in_DEG(void);
			Eigen::Vector3d getVbody(void);
			Eigen::Vector3d getLocalAngle(void);

			void update_gravitiy(Eigen::Vector3d);
			
			friend std::ostream& operator<<(std::ostream& os, const strapdown_ecef&);
			friend  strapdown_ecef& operator>>(strapdown_ecef&, PositionData&);

		private:
			Eigen::Matrix3d strapdown_ecef::skew(Eigen::Vector3d&);
			Eigen::Vector3d strapdown_ecef::transform_ecef2llh_DEG(Eigen::Vector3d&);
			Eigen::Vector3d strapdown_ecef::transform_ecef2llh_RAD(Eigen::Vector3d&);
			Eigen::Matrix3d strapdown_ecef::pos2Cne_RAD(double&, double&);
			Eigen::Matrix3d strapdown_ecef::pos2Cne_RAD(Eigen::Vector3d&);
			Eigen::Matrix3d strapdown_ecef::pos2Cne_ECEF(Eigen::Vector3d&);
			Eigen::Matrix3d strapdown_ecef::getCbe(void);
			Eigen::Matrix3d strapdown_ecef::calculateCbe(Eigen::Vector3d, Eigen::Vector3d);
			
	};
}