
#include "IMUSignalGenerator.h"

namespace IMUSimulator {


	IMUSignalGenerator::IMUSignalGenerator(void) {

	}

	void IMUSignalGenerator::update_gravitiy(double lat_deg, double lon_deg, double h) {

		// lat long in DEG
		double llh_array_deg[3];
		llh_array_deg[0] = lat_deg;
		llh_array_deg[1] = lon_deg;
		llh_array_deg[2] = h;

		this->wgs84.setCoordinates(llh_array_deg, IMUSimulator::CoordiateFrame::LLH_Frame);
		this->wgs84.get_Gravity_and_WIE_E(this->g, this->wie_e);
	}

	void IMUSignalGenerator::addError(void) {
	
	
	}

	Measure_IMU IMUSignalGenerator::calculate(Eigen::Vector3d a_body, Eigen::Vector3d w_body, Eigen::Vector3d V_body, Eigen::Vector3d llh_deg, Eigen::Vector3d attitude) {
		// llh in DEG
		update_gravitiy(llh_deg[0], llh_deg[1], llh_deg[2]);
		Eigen::Vector3d llh_rad;
		llh_rad << llh_deg[0] * EIGEN_PI / 180., llh_deg[1] * EIGEN_PI / 180., llh_deg[2];

		Eigen::Vector3d Vn, Ve, gn, an, ae, wn, we, an_rot, ae_coriolis, wn_rot, we_rot, ge, ab_cf, ae_cf;

		Eigen::Matrix3d Cbn = IMUSimulator::Lib::euler2dcm(attitude);
		Eigen::Matrix3d Cne = pos2Cne_RAD(llh_rad[0], llh_rad[1]);

		gn << 0, 0, -g;
		ge = Cne * gn;

		Ve = Cne * (Cbn * V_body);
		ae = Cne * (Cbn * a_body);
		we = Cne * (Cbn * w_body);

		//an += wn.cross(Vn); // Centripetal acc

		we_rot << 0, 0, wie_e;
		//wn_rot = Cne.transpose()*we_rot;

		//an_rot = wn.cross(Vn); // acceleration from body rotation. centripetal force -sign?
		ae_coriolis =  2*we_rot.cross(Ve);
		//an_rot += wn_rot.cross(wn_rot.cross(Vn))
		
		//body feels the centrifugal force due to turning
		ab_cf << 0., 0., 0.;
		ab_cf= w_body.cross(V_body);
		ae_cf = Cne * Cbn * ab_cf;

		ae += ge + ae_coriolis + ae_cf;

		a_body = Cbn.transpose()*(Cne.transpose() * ae);
		w_body += Cbn.transpose()*Cne.transpose()*we_rot;

		meas.a[0] = a_body(0);
		meas.a[1] = a_body(1);
		meas.a[2] = a_body(2);

		meas.w[0] = w_body(0);
		meas.w[1] = w_body(1);
		meas.w[2] = w_body(2);

		return meas;
	}

	Eigen::Matrix3d IMUSignalGenerator::pos2Cne_RAD(double& lat, double& lon) {
		// lat long in rad
		return IMUSimulator::Lib::pos2Cne(lat, lon);
	}

}