#include "strapdown_ECEF.h"
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
		
		llh_new = this->transform_ecef2llh(ecef_new);
		this->Cne = this->pos2Cne(llh_new[0], llh_new[1]);

		Eigen::Matrix3d Cbe_new;
		Cbe_new = this->Cne;

		/*Set new params*/
		this->setParams(Cbe_new, Ve_new, ecef_new);
	}

	strapdown_ecef::strapdown_ecef(Eigen::Vector3d& Ve_new, Eigen::Vector3d& ecef_new) {

		/*Calculate default Cbe from Cne*/
		Eigen::Vector3d llh_new;

		llh_new = transform_ecef2llh(ecef_new);
		Cne = pos2Cne(llh_new[0], llh_new[1]);
		Eigen::Matrix3d Cbe_new;
		Cbe_new = Cne;

		/*Set new params*/
		this->setParams(Cbe_new, Ve_new, ecef_new);
	}

	
	strapdown_ecef::strapdown_ecef(Eigen::Vector3d& rollpitchyaw, Eigen::Vector3d& Vb, Eigen::Vector3d& ecef_new) {

		Cnb = IMUSimulator::Lib::euler2dcm(rollpitchyaw);
		Eigen::Vector3d Ve_new;
		Eigen::Matrix3d Cbn = Cnb.transpose();  // TODO - Maybe euler2dcm2 give Cbn and not Cnb

		Eigen::Vector3d llh_new = transform_ecef2llh(ecef_new);
		Cne = pos2Cne(llh_new[0], llh_new[1]);

		Cbe = Cne * Cbn;
		Ve_new = Cbe*Vb;

		/*Set new params*/
		this->setParams(Cbe, Ve_new, ecef_new);
	}

	strapdown_ecef::strapdown_ecef(Eigen::Matrix3d& Cbe_new, Eigen::Vector3d& Ve_new, Eigen::Vector3d& ecef_new) {

		/*Set new params*/
		this->setParams(Cbe_new, Ve_new, ecef_new);
		//this->wgs84.setWGS84Params();
	}

	/*Update step*/
	void strapdown_ecef::update(Eigen::Vector3d& rollpitchyaw, Eigen::Vector3d& Ve, Eigen::Vector3d& ecef, Eigen::Vector3d&a, Eigen::Vector3d& w, double dt) {
		
		Eigen::Matrix3d Cbn = IMUSimulator::Lib::euler2dcm(rollpitchyaw);

		Cnb = Cbn.transpose();
		Cbe = Cne * Cbn;			
		update(Cbe, Ve, ecef, a, w, dt);
	}

	void strapdown_ecef::update(Eigen::Matrix3d& Cbe, Eigen::Vector3d& Ve, Eigen::Vector3d& ecef, Eigen::Vector3d&a, Eigen::Vector3d& w, double dt) {
		
		this->setParams(Cbe, Ve, ecef);
		this->update(a, w, dt);
	}
	
	void strapdown_ecef::update(Measure_IMU& meas, double dt) {
	
		Eigen::Vector3d a, w;
		a << meas.a[0], meas.a[1], meas.a[2];
		w << meas.w[0], meas.w[1], meas.w[2];

		update(a, w, dt);
	}

	void strapdown_ecef::update(Eigen::Vector3d& a, Eigen::Vector3d& w, double dt) {

		this->update_gravitiy(llh);

		Eigen::Vector3d gn, ge;
		Eigen::Vector3d rot_Earth_n;
		rot_Earth_n << 0, 0, -wie_e*dt;

		double sr_a, sr_b, rot_norm;
		Eigen::Matrix3d eye = Eigen::Matrix3d::Identity();
		Eigen::Matrix3d	mx_a, mx_b,
						rot_skew;
		Eigen::Vector3d	vel_inc1, vel_inc2;

		Eigen::Vector3d ecef_new, Ve_new;

		this->Cne = this->pos2Cne(this->llh[0], this->llh[1]);

		gn << 0, 0, g;
		ge =  - Cne * gn; //TODO sahll there be  a "-" ??

		/*Update attitude*/

		/*Body rotation*/
		Eigen::Vector3d rot = w * dt;
		rot_skew = skew(rot);
		rot_norm = rot.norm();

		sr_a = 1 - (pow(rot_norm,2)  / 6) + (pow(rot_norm,4) / 120);
		sr_b = (1 / 2) - (pow(rot_norm, 2) / 24) + (pow(rot_norm, 4) / 720);
		mx_a = eye + sr_a*rot_skew + sr_b*rot_skew*rot_skew;
		
		/*Earth rotation*/
		rot_skew = skew(rot_Earth_n);
		rot_norm = rot_Earth_n.norm();
		sr_a = 1 - (pow(rot_norm, 2) / 6) + (pow(rot_norm, 4) / 120);
		sr_b = (1 / 2) - (pow(rot_norm, 2) / 24) + (pow(rot_norm, 4) / 720);
		mx_b = eye + sr_a*rot_skew + sr_b*rot_skew*rot_skew;

		Cbe = mx_b*Cbe*mx_a;
		
		/*Update Cnb matrix. navigation to body*/
		Cnb = Cne.transpose()*Cbe;

		/*Update Velocity*/
		vel_inc1 = Cbe * a *dt;
		vel_inc2 = (-ge + 2 * Ve.cross(rot_Earth_n))*dt;
		Ve = Ve + vel_inc1 + vel_inc2;

		//Update_pos
		ecef = ecef + Ve*dt;
	}

	void strapdown_ecef::setParams(Eigen::Matrix3d& Cbe, Eigen::Vector3d& Ve, Eigen::Vector3d& ecef_new ) {

		this->Cbe = Cbe;
		this->Ve = Ve;

		this->ecef[0] = ecef_new[0];
		this->ecef[1] = ecef_new[1];
		this->ecef[2] = ecef_new[2];

		this->llh = this->transform_ecef2llh(ecef_new );
	}

	Eigen::Vector3d strapdown_ecef::getLLH(void){
		return llh;
	}

	Eigen::Vector3d strapdown_ecef::getVbody(void) {
	
		return Cbe.transpose() * Ve;
	}

	void strapdown_ecef::update_gravitiy(Eigen::Vector3d llh) {

		double llh_array[3];
		llh_array[0] = llh[0];
		llh_array[1] = llh[1];
		llh_array[2] = llh[2];

		this->wgs84.setCoordinates(llh_array, IMUSimulator::CoordiateFrame::LLH_Frame);
		this->wgs84.get_Gravity_and_WIE_E(this->g, this->wie_e);
	}

	std::ostream& operator<<(std::ostream& os, const strapdown_ecef& str_e) {
	
		os << "LLH: " << std::endl << str_e.llh << std::endl;
		os << "ECEF: " << std::endl << str_e.ecef << std::endl;

		os << std::endl;

		os << "Ve: " << std::endl << str_e.Ve << std::endl;
		os << "Vb: " << std::endl << str_e.Cbe.transpose()*str_e.Ve << std::endl;
		os << "Vn: " << std::endl << str_e.Cne.transpose()*str_e.Ve << std::endl;

		os << std::endl;
		
		os << "Cbe: " << std::endl << str_e.Cbe << std::endl;
		os << "Cne: " << std::endl << str_e.Cne << std::endl;
		os << "Cnb: " << std::endl << str_e.Cnb << std::endl;

		Eigen::Vector3d local_angle = IMUSimulator::Lib::dcm2euler(str_e.Cnb);
		os << "Roll pitch yaw: " << std::endl << local_angle << std::endl;
		
		return os;
	}

	/*Utility methods*/
	Eigen::Matrix3d strapdown_ecef::pos2Cne(double& lat, double& lon) {
	
		return IMUSimulator::Lib::pos2Cne(lat, lon);
	}

	Eigen::Vector3d strapdown_ecef::transform_ecef2llh(Eigen::Vector3d& ecef) {

		return IMUSimulator::Lib::transform_ecef2llh(ecef);
	}

	Eigen::Matrix3d strapdown_ecef::skew(Eigen::Vector3d& v) {
	
		return IMUSimulator::Lib::skew(v);
	}
}
