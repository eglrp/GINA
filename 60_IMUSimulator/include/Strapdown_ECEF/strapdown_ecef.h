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
			strapdown_ecef(void);
			strapdown_ecef(double, double, double);
			strapdown_ecef(Eigen::Vector3d&);
			strapdown_ecef(Eigen::Vector3d&, Eigen::Vector3d&);
			strapdown_ecef(Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&);
			strapdown_ecef(double[3], double[3], double[3]);

			void update(Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&, double);
			void strapdown_ecef::update(Measure_IMU&, double);
			void update(Eigen::Vector3d&, Eigen::Vector3d&, double);

			void setParams(Eigen::Vector3d&, Eigen::Vector3d&, Eigen::Vector3d&);
			void setParams(double[3], double[3], double[3]);
			void setTime(unsigned int&, double&);

			Eigen::Vector3d getLLH_in_DEG(void) const;
			Eigen::Vector3d getVbody(void) const;
			Eigen::Vector3d getLocalAngle(void) const;

			void update_gravitiy(Eigen::Vector3d);
			
			strapdown_ecef operator=(const strapdown_ecef& str_e);

			friend std::ostream& operator<<(std::ostream& os, const strapdown_ecef&);
			friend  strapdown_ecef& operator>>(strapdown_ecef&, PositionData&);

		private:
			Eigen::Matrix3d strapdown_ecef::skew(const Eigen::Vector3d&) const;
			Eigen::Vector3d strapdown_ecef::transform_ecef2llh_DEG(const Eigen::Vector3d&) const;
			Eigen::Vector3d strapdown_ecef::transform_ecef2llh_DEG(const double[3]) const;
			Eigen::Vector3d strapdown_ecef::transform_ecef2llh_RAD(const Eigen::Vector3d&) const;
			Eigen::Matrix3d strapdown_ecef::pos2Cne_RAD(const double&, const double&) const;
			Eigen::Matrix3d strapdown_ecef::pos2Cne_RAD(const Eigen::Vector3d&) const;
			Eigen::Matrix3d strapdown_ecef::pos2Cne_ECEF(const Eigen::Vector3d&) const;
			Eigen::Matrix3d strapdown_ecef::getCbe(void) const;
			Eigen::Matrix3d strapdown_ecef::calculateCbe(const Eigen::Vector3d, const Eigen::Vector3d) const;
			
	};
}