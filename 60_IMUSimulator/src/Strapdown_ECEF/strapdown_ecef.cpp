#include "strapdown_ecef.h"
#include "std_IMUSimulator.h"

#include "Position.hpp"
#include "WGS84Ellipsoid.hpp"

#include "lib_Frames.h"

namespace IMUSimulator {

	/*Constructors*/
	strapdown_ecef::strapdown_ecef(Eigen::Vector3d& ecef_new) {

		/*Set default velocity*/
		Eigen::Vector3d Ve_new;
		Ve_new << 0, 0, 0;

		/*Calculate default Cbe from Cne*/
		Eigen::Vector3d llh_new;
		
		llh_new = this->transform_ecef2llh_DEG(ecef_new);
		//this->Cne = this->pos2Cne(llh_new[0], llh_new[1]);

		//Eigen::Matrix3d Cbe_new;
		//Cbe_new = this->Cne;

		Eigen::Vector3d attitude;
		attitude << 0., 0., 0.;

		/*Set new params*/
		this->setParams(attitude, Ve_new, ecef_new);
	}

	strapdown_ecef::strapdown_ecef(Eigen::Vector3d& Ve_new, Eigen::Vector3d& ecef_new) {

		/*Calculate default Cbe from Cne*/
		//Eigen::Vector3d llh_new;

		//llh_new = transform_ecef2llh_DEG(ecef_new);
		//Cne = pos2Cne(llh_new[0], llh_new[1]);
		//Eigen::Matrix3d Cbe_new;
		//Cbe_new = Cne;

		Eigen::Vector3d attitude;
		attitude << 0., 0., 0.;

		/*Set new params*/
		this->setParams(attitude, Ve_new, ecef_new);
	}

	strapdown_ecef::strapdown_ecef(Eigen::Vector3d& attitude, Eigen::Vector3d& Vb, Eigen::Vector3d& ecef_new) {

		// TODO it can be done simpler
		Eigen::Matrix3d Cbn = IMUSimulator::Lib::euler2dcm(attitude);

		Eigen::Vector3d llh_new = transform_ecef2llh_DEG(ecef_new);
		llh_new *= EIGEN_PI / 180.0;
		Eigen::Matrix3d Cne = pos2Cne_RAD(llh_new[0], llh_new[1]);

		Eigen::Matrix3d Cbe = Cne * Cbn;
		Eigen::Vector3d Ve_new = Cbe*Vb;

		/*Set new params*/
		this->setParams(attitude, Ve_new, ecef_new);

	}

	/*Update step*/
	void strapdown_ecef::update(Eigen::Vector3d& attitude, Eigen::Vector3d& Ve, Eigen::Vector3d& ecef, Eigen::Vector3d& ab, Eigen::Vector3d& wb, double dt) {

		this->setParams(attitude, Ve, ecef);
		update(ab, wb, dt);
	}
	
	void strapdown_ecef::update(Measure_IMU& meas, double dt) {

		double newtow = meas.tow + dt;
		setTime(meas.wn, newtow);

		Eigen::Vector3d ab, wb;
		ab << meas.a[0], meas.a[1], meas.a[2];
		wb << meas.w[0], meas.w[1], meas.w[2];

		update(ab, wb, dt);
	}

	void strapdown_ecef::update(Eigen::Vector3d& ab, Eigen::Vector3d& wb, double dt) {

		this->update_gravitiy(transform_ecef2llh_DEG(this->ecef));

		Eigen::Vector3d gn, ge;
		Eigen::Vector3d rot_Earth_n;
		//rot_Earth_n << 0, 0, -wie_e*dt;
		rot_Earth_n << 0, 0, wie_e;

		double sr_a, sr_b, rot_norm;
		Eigen::Matrix3d eye = Eigen::Matrix3d::Identity();
		Eigen::Matrix3d	mx_a, mx_b,
						rot_skew;
		Eigen::Vector3d	vel_inc1, vel_inc2, temp;

		Eigen::Vector3d ecef_new, Ve_new, Vb, Ve_update;

		//this->llh = this->transform_ecef2llh(ecef);
		//this->Cne = this->pos2Cne(this->llh[0], this->llh[1]);
		Eigen::Matrix3d Cne = this->pos2Cne_ECEF(ecef);

		gn << 0, 0, -g;
		ge =   Cne * gn; 

		/*Update attitude*/

		/*Body rotation*/
		Eigen::Vector3d rot = wb * dt;
		rot_skew = skew(rot);
		rot_norm = rot.norm();

		sr_a = 1 - (pow(rot_norm,2)  / 6) + (pow(rot_norm,4) / 120);
		sr_b = (1 / 2) - (pow(rot_norm, 2) / 24) + (pow(rot_norm, 4) / 720);
		mx_a = eye + sr_a*rot_skew + sr_b*rot_skew*rot_skew;
		
		/*Earth rotation*/
		rot = -rot_Earth_n * dt;
		//rot_skew = skew(rot_Earth_n);
		rot_skew = skew(rot);
		rot_norm = rot.norm();
		sr_a = 1 - (pow(rot_norm, 2) / 6) + (pow(rot_norm, 4) / 120);
		sr_b = (1 / 2) - (pow(rot_norm, 2) / 24) + (pow(rot_norm, 4) / 720);
		mx_b = eye + sr_a*rot_skew + sr_b*rot_skew*rot_skew;

		// Update Cbe matrix
		Eigen::Matrix3d Cbe = getCbe();
		Cbe = mx_b*(Cbe*mx_a);
		
		/*Update Cnb matrix. navigation to body*/
		Eigen::Matrix3d Cbn = Cne.transpose()*Cbe;

		this->rollpitchyaw = IMUSimulator::Lib::dcm2euler(Cbn); // TODO maybe we shall use 

		temp = IMUSimulator::Lib::dcm2euler(Cne)*180./EIGEN_PI;

		/*Update Velocity*/
		vel_inc1 = Cbe * ab *dt;
		vel_inc2 = (-ge + 2 * Ve.cross(rot_Earth_n))*dt;
		Ve_update = vel_inc1 + vel_inc2;
		Ve += Ve_update;

		Vb = Cbe.transpose() * Ve;

		//Update_pos
		ecef = ecef + Ve*dt;
	}

	void strapdown_ecef::setParams(Eigen::Vector3d& attitude, Eigen::Vector3d& Ve, Eigen::Vector3d& ecef_new ) {

		//this->Cbe = Cbe;
		this->Ve = Ve;

		this->ecef[0] = ecef_new[0];
		this->ecef[1] = ecef_new[1];
		this->ecef[2] = ecef_new[2];
		rollpitchyaw = attitude;
		//this->Cne = this->pos2Cne(ecef);
		//this->llh = this->transform_ecef2llh(ecef_new );
	}

	const Eigen::Vector3d strapdown_ecef::getLLH_in_DEG(void){
		return transform_ecef2llh_DEG(this->ecef);
	}

	Eigen::Vector3d strapdown_ecef::getVbody(void) {
	
		Eigen::Matrix3d Cbn = IMUSimulator::Lib::euler2dcm(this->rollpitchyaw);

		Eigen::Matrix3d Cne = this->pos2Cne_ECEF(this->ecef);
		Eigen::Matrix3d Cbe = Cne * Cbn;

		return Cbe.transpose() * Ve;
	}

	Eigen::Vector3d strapdown_ecef::getLocalAngle(void) {
	
		return rollpitchyaw;
	}

	void strapdown_ecef::update_gravitiy(Eigen::Vector3d llh_deg) {

		// llh in DEG

		double llh_array_deg[3];
		llh_array_deg[0] = llh_deg[0];
		llh_array_deg[1] = llh_deg[1];
		llh_array_deg[2] = llh_deg[2];

		this->wgs84.setCoordinates(llh_array_deg, IMUSimulator::CoordiateFrame::LLH_Frame);
		this->wgs84.get_Gravity_and_WIE_E(this->g, this->wie_e);
	}

	std::ostream& operator<<(std::ostream& os, strapdown_ecef& str_e) {
	
		Eigen::Vector3d llh;
		llh = str_e.getLLH_in_DEG();
		os << "LLH: " << std::endl << llh << std::endl;
		os << "ECEF: " << std::endl << str_e.ecef << std::endl;

		os << std::endl;

		os << "Ve: " << std::endl << str_e.Ve << std::endl;
		os << "Vb: " << std::endl << str_e.getVbody() << std::endl;
		//os << "Vn: " << std::endl << str_e.Cne.transpose()*str_e.Ve << std::endl;

		os << std::endl;
		
		//os << "Cbe: " << std::endl << str_e.Cbe << std::endl;
		//os << "Cne: " << std::endl << str_e.Cne << std::endl;
		//os << "Cnb: " << std::endl << str_e.Cnb << std::endl;

		//Eigen::Vector3d local_angle = IMUSimulator::Lib::dcm2euler(str_e.Cnb);
		os << "Roll pitch yaw: " << std::endl << str_e.rollpitchyaw << std::endl;
		
		return os;
	}

	strapdown_ecef& operator>>(strapdown_ecef& str_e, PositionData& pos) {

		pos.ecef[0] = str_e.ecef[0];
		pos.ecef[1] = str_e.ecef[1];
		pos.ecef[2] = str_e.ecef[2];

		pos.attitude[0] = str_e.rollpitchyaw[0];
		pos.attitude[1] = str_e.rollpitchyaw[1];
		pos.attitude[2] = str_e.rollpitchyaw[2];
		
		pos.GPSWeek = str_e.GPSWeek;
		pos.GPSToW = str_e.GPSToW;
		return str_e;
	}

	void strapdown_ecef::setTime(unsigned int& wn, double& tow) {

		// TODO week rollover
		this->GPSWeek = wn;
		this->GPSToW = tow;
	}

	/*Utility methods*/
	Eigen::Matrix3d strapdown_ecef::pos2Cne_RAD(double& lat, double& lon) {
		// lat long in rad
		return IMUSimulator::Lib::pos2Cne(lat, lon);
	}

	Eigen::Matrix3d strapdown_ecef::pos2Cne_RAD(Eigen::Vector3d& llh) {
		// lat long in rad
		return IMUSimulator::Lib::pos2Cne(llh(0), llh(1));
	}

	Eigen::Matrix3d strapdown_ecef::pos2Cne_ECEF(Eigen::Vector3d& ecef) {
		// lat long shall be in rad
		Eigen::Vector3d llh = this->transform_ecef2llh_DEG(this->ecef)*EIGEN_PI/180.0;
		return IMUSimulator::Lib::pos2Cne(llh(0), llh(1));
	}

	Eigen::Vector3d strapdown_ecef::transform_ecef2llh_DEG(Eigen::Vector3d& ecef) {

		return IMUSimulator::Lib::transform_ecef2llh(ecef);
	}

	Eigen::Vector3d strapdown_ecef::transform_ecef2llh_RAD(Eigen::Vector3d& ecef) {

		return IMUSimulator::Lib::transform_ecef2llh(ecef) *EIGEN_PI / 180;
	}

	Eigen::Matrix3d strapdown_ecef::skew(Eigen::Vector3d& v) {
	
		return IMUSimulator::Lib::skew(v);
	}

	
	Eigen::Matrix3d strapdown_ecef::getCbe(void) {

		return calculateCbe(rollpitchyaw, ecef);
	}

	Eigen::Matrix3d strapdown_ecef::calculateCbe(Eigen::Vector3d attitude, Eigen::Vector3d ecef_in) {

		// TODO it can be done simpler
		Eigen::Matrix3d Cbn = IMUSimulator::Lib::euler2dcm(attitude);

		Eigen::Vector3d llh_new = transform_ecef2llh_DEG(ecef_in)*EIGEN_PI/180.0;
		Eigen::Matrix3d Cne = pos2Cne_RAD(llh_new[0], llh_new[1]);

		return Cne * Cbn;
	}
}
