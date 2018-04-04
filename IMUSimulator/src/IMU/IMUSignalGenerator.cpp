
#include "IMUSignalGenerator.h"

namespace IMUSimulator {


	IMUSignalGenerator::IMUSignalGenerator(void) {

	}

	void IMUSignalGenerator::update_gravitiy(double lat, double lon, double h) {

		double llh_array[3];
		llh_array[0] = lat;
		llh_array[1] = lon;
		llh_array[2] = h;

		this->wgs84.setCoordinates(llh_array, IMUSimulator::CoordiateFrame::LLH_Frame);
		this->wgs84.get_Gravity_and_WIE_E(this->g, this->wie_e);
	}

	void IMUSignalGenerator::addError(void) {
	
	
	}

	Measure_IMU IMUSignalGenerator::calculate(Eigen::Vector3d a_body, Eigen::Vector3d w_body, Eigen::Vector3d V_body, Eigen::Vector3d llh, Eigen::Vector3d attitude) {
	
		update_gravitiy(llh[0], llh[1], llh[2]);

		Eigen::Vector3d Vn, gn, an, wn, an_rot, wn_rot;

		Eigen::Matrix3d Cnb = IMUSimulator::Lib::euler2dcm(attitude);
		Eigen::Matrix3d Cne = pos2Cne(llh[0], llh[1]);

		Eigen::Matrix3d Cbn = Cnb.transpose();

		gn << 0, 0, -g;

		Vn = Cbn * V_body;
		an = Cbn * a_body;
		wn = Cbn * w_body;

		an_rot = wn.cross(Vn); // TODO coriolli force is missing
		wn_rot << 0, 0, wie_e;

		an += gn + an_rot;
		wn += wn_rot;

		a_body = Cnb*an;
		w_body = Cnb*wn;

		meas.a[0] = a_body(0);
		meas.a[1] = a_body(1);
		meas.a[2] = a_body(2);

		meas.w[0] = w_body(0);
		meas.w[1] = w_body(1);
		meas.w[2] = w_body(2);

		return meas;
	}

	Eigen::Matrix3d IMUSignalGenerator::pos2Cne(double& lat, double& lon) {

		return IMUSimulator::Lib::pos2Cne(lat, lon);
	}

}