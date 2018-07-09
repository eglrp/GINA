#include "geoparam_wgs84.h"

#include "Position.hpp"
#include "GPSWeekSecond.hpp"
#include "WGS84Ellipsoid.hpp"

namespace IMUSimulator{

	WGS84Ellipsoid::WGS84Ellipsoid(void) {
		this->setWGS84Params();
	}

	WGS84Ellipsoid::WGS84Ellipsoid(const WGS84Ellipsoid &wgs84) {

		this->llh[0] = wgs84.llh[0];
		this->llh[1] = wgs84.llh[1];
		this->llh[2] = wgs84.llh[2];

		this->WIE_E = wgs84.WIE_E;
		this->SM_AXIS = wgs84.SM_AXIS;
		this->E_SQR = wgs84.E_SQR;
		this->NORMAL_GRV = wgs84.NORMAL_GRV;
		this->GRV_CONS = wgs84.GRV_CONS;
		this->FLATTENING = wgs84.FLATTENING;
		this->M_FAKTOR = wgs84.M_FAKTOR;

	}

	WGS84Ellipsoid::WGS84Ellipsoid(double Coor[], CoordiateFrame Type) {

		gpstk::Position LLH;
		

		this->setWGS84Params();
		this->setCoordinates(Coor, Type);
		
	}

	WGS84Ellipsoid& WGS84Ellipsoid::operator=(const WGS84Ellipsoid& wgs84) {
	
		this->llh[0] = wgs84.llh[0];
		this->llh[1] = wgs84.llh[1];
		this->llh[2] = wgs84.llh[2];

		this->WIE_E = wgs84.WIE_E;
		this->SM_AXIS = wgs84.SM_AXIS;
		this->E_SQR = wgs84.E_SQR;
		this->NORMAL_GRV = wgs84.NORMAL_GRV;
		this->GRV_CONS = wgs84.GRV_CONS;
		this->FLATTENING = wgs84.FLATTENING;
		this->M_FAKTOR = wgs84.M_FAKTOR;

		return *this;
	}

	void WGS84Ellipsoid::setWGS84Params(void) {
		gpstk::WGS84Ellipsoid wgs84;

		this->WIE_E = wgs84.angVelocity();					// 7292115e-11;    //earth's rotaion rate
		this->SM_AXIS = wgs84.a();							//6378137;
		this->E_SQR = wgs84.eccSquared();					// 0.00669437999014 = 8.1819190842622e-2^2
		this->NORMAL_GRV = 9.7803253359;
		this->GRV_CONS = 0.00193185265241;
		this->FLATTENING = wgs84.flattening();				// 0.00335281066475;
		this->M_FAKTOR = 0.00344978650684;
	}

	void WGS84Ellipsoid::setCoordinates(double Coor[], CoordiateFrame Type) {
	
		gpstk::WGS84Ellipsoid wgs84;
		try {
			if (Type == ECEF_Frame) {
				//conversion to LLH
				gpstk::Position Coordinates(Coor[0],
											Coor[1],
											Coor[2],
											gpstk::Position::CoordinateSystem::Cartesian,
											&wgs84,
											gpstk::ReferenceFrame::WGS84);

				Coordinates.asGeodetic();

				this->llh[0] = Coordinates.geodeticLatitude();
				this->llh[1] = Coordinates.getLongitude();
				this->llh[2] = Coordinates.getHeight();
			}
			else if (Type == LLH_Frame) {
				
				this->llh[0] = Coor[0];
				this->llh[1] = Coor[1];
				this->llh[2] = Coor[2];
			}
			else {
				//throw warning
				throw gpstk::GeometryException("The incoming coordinates neither are in ECEF or LLH\n");
			}
		}
		catch (gpstk::GeometryException& e) {
			this->llh[0] = 0;
			this->llh[1] = 0;
			this->llh[2] = 0;
		}
	}

	void WGS84Ellipsoid::getParams(double& Rn, double& Re, double& g, double& sL, double& cL, double& wie_e) {
		
		sL = std::sin(this->llh[0] * DEG2RAD);
		cL = std::cos(this->llh[0] * DEG2RAD);
		double h = this->llh[2] * DEG2RAD;
		Rn = 6335439.327292829 / (std::sqrt(1.0 - E_SQR*sL*sL)*(1.0 - E_SQR*sL*sL));
		Re = SM_AXIS / (std::sqrt(1.0 - E_SQR*sL*sL));
		double g1 = NORMAL_GRV*(1 + GRV_CONS*sL*sL) / (std::sqrt(1.0 - E_SQR*sL*sL));
		g = g1*(1.0 - (2.0 / SM_AXIS)*(1.0 + FLATTENING + M_FAKTOR - 2.0*FLATTENING*sL*sL)*h + 3.0*h*h / SM_AXIS / SM_AXIS);

		wie_e = this->WIE_E;
	};

	void WGS84Ellipsoid::get_Gravity_and_WIE_E(double& g, double& wie) {

		double Rn, Re, sL, cL;
		this->getParams(Rn, Re, g, sL, cL, wie);
	}
}