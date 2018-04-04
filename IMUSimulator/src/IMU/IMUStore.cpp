
#include "IMUStore.h"

namespace IMUSimulator {

	IMUStore::IMUStore(void) {

	}

	void IMUStore::add_measure(double ax, double ay, double az, double wx, double wy, double wz, double wn, double tow) {

		Measure_IMU meas;

		meas.wn = wn;
		meas.tow = tow;

		meas.a[0] = ax;
		meas.a[1] = ay;
		meas.a[2] = az;

		meas.w[0] = wx;
		meas.w[1] = wy;
		meas.w[2] = wz;

		add_measure(IMUSimulator::TimeFrame::GPSTime, meas);
	}

	void IMUStore::add_measure(TimeFrame timeframe, Measure_IMU meas) {
	
		if (timeframe == IMUSimulator::TimeFrame::GPSTime) {

			node.wn = meas.wn;
			node.tow = meas.tow;
		}
		else {

			node.wn = 0;
			node.tow = 0;
			// throw exception
		}

		node.a[0] = meas.a[0];
		node.a[1] = meas.a[1];
		node.a[2] = meas.a[2];

		node.w[0] = meas.w[0];
		node.w[1] = meas.w[1];
		node.w[2] = meas.w[2];

		add_measure();
	}

	void IMUStore::add_measure(void) {

		measurement.push_back(node);
		clearNode();
	}

	void IMUStore::clearNode(void) {

		node.wn = 0;
		node.tow = 0.0;

		node.a[0] = 0.0;
		node.a[1] = 0.0;
		node.a[2] = 0.0;

		node.w[0] = 0.0;
		node.w[1] = 0.0;
		node.w[2] = 0.0;
	}

	std::ostream& operator<<(std::ostream& os, IMUStore& imuStore) {

		for (std::vector<Measure_IMU>::iterator it = imuStore.measurement.begin(); it != imuStore.measurement.end(); ++it) {
			os << *it;
		}

		return os;
	}

	void IMUStore::printf(std::ostream& os) {

		for (std::vector<Measure_IMU>::iterator it = measurement.begin(); it != measurement.end(); ++it) {
			os << *it;
		}
	}

	std::ostream& operator<<(std::ostream& os, const Measure_IMU& meas) {

		os << "Time: " << "WN: " << meas.wn << "   ToW: " << meas.tow << std::endl;
		os << "Acceleration [x,y,z]: " << meas.a[0] << " " << meas.a[1] << " " << meas.a[2] << "	";
		os << "Angular rate [wx,wy,wz]: " << meas.w[0] << " " << meas.w[1] << " " << meas.w[2] << std::endl;
		os << std::endl << std::endl;

		return os;
	}

	void IMUStore::updateTime(TimeFrame timeframe, int wn, double tow) {

		if (timeframe == IMUSimulator::TimeFrame::GPSTime) {
			measurement.back().tow = tow;
			measurement.back().wn = wn;
		}
		else {
			throw("Invalid TimeFrame given");
		}
	}

}