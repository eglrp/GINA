
#include "lib_Frames.h"
#include "geoparam_wgs84.h"
#include "Position.hpp"
#include "WGS84Ellipsoid.hpp"


namespace IMUSimulator {
	namespace Lib {


		Eigen::Vector3d transform_ecef2llh(Eigen::Vector3d& ecef) {

			Eigen::Vector3d llh;
			gpstk::WGS84Ellipsoid wgs84;
			gpstk::Position Coordinates(ecef[0],
				ecef[1],
				ecef[2],
				gpstk::Position::CoordinateSystem::Cartesian,
				&wgs84,
				gpstk::ReferenceFrame::WGS84);

			Coordinates.asGeodetic();

			llh[0] = Coordinates.getGeodeticLatitude();
			llh[1] = Coordinates.getLongitude();
			llh[2] = Coordinates.getHeight();

			return llh;

		}

		Eigen::Vector3d transform_llh2ecef(Eigen::Vector3d& llh) {

			Eigen::Vector3d ecef;
			gpstk::WGS84Ellipsoid wgs84;
			gpstk::Position Coordinates(llh[0],
										llh[1],
										llh[2],
										gpstk::Position::CoordinateSystem::Geodetic,
										&wgs84,
										gpstk::ReferenceFrame::WGS84);

			Coordinates.asECEF();

			ecef[0] = Coordinates.getX();
			ecef[1] = Coordinates.getY();
			ecef[2] = Coordinates.getZ();

			return ecef;

		}

		Eigen::Matrix3d pos2Cne(double& lat, double& lon) {

			Eigen::Matrix3d Cne;

			double sL = std::sin(lat);
			double cL = std::cos(lat);
			double sl = std::sin(lon);
			double cl = std::cos(lon);

			Cne << -sL*cl, -sl, -cL*cl,
				-sL*sl, cl, -cL*sl,
				cL, 0, -sL;

			return Cne;
		}

		Eigen::Matrix3d skew(Eigen::Vector3d& v) {
			Eigen::Matrix3d m;
			m <<	0,		-v[2],	 v[1],
					v[2],	 0,		-v[0],
				   -v[1],	 v[0],	 0;
			return m;
		}

	}
}