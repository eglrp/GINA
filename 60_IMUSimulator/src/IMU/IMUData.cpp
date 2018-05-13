
#include "IMUData.h"

namespace IMUSimulator {

	IMUData::IMUData(void) {

	}

	IMUData& IMUData::operator=(Measure_IMU& node) {
	
		

		this->acceleration[0] = node.a[0];
		this->acceleration[1] = node.a[1];
		this->acceleration[2] = node.a[2];

		this->angularRate[0] = node.w[0];
		this->angularRate[1] = node.w[1];
		this->angularRate[2] = node.w[2];

		this->GPSWeek = node.wn;
		this->GPSToW = node.tow;

		return *this;
	}

	Measure_IMU& IMUData::operator=(IMUData& node) {

		Measure_IMU new_node;

		new_node.a[0] = node.acceleration[0];
		new_node.a[1] = node.acceleration[1];
		new_node.a[2] = node.acceleration[2];

		new_node.w[0] = node.angularRate[0];
		new_node.w[1] = node.angularRate[1];
		new_node.w[2] = node.angularRate[2];

		new_node.wn = node.GPSWeek;
		new_node.tow = node.GPSToW;

		return new_node;
	}

	IMUData& IMUData::operator=(PINASimulator::IMUData& node) {
		
		IMUData new_node;
		
		new_node.acceleration[0] = node.acceleration[0];
		new_node.acceleration[1] = node.acceleration[1];
		new_node.acceleration[2] = node.acceleration[2];

		new_node.angularRate[0] = node.angularRate[0];
		new_node.angularRate[1] = node.angularRate[1];
		new_node.angularRate[2] = node.angularRate[2];

		new_node.GPSWeek = node.getGPSWeek();
		new_node.GPSToW = node.getGPSToW();
		
		return new_node;
	}

	double IMUData::getAccX(void) {
		return acceleration[0];
	}
	double IMUData::getAccY(void) {
		return acceleration[1];
	}
	double IMUData::getAccZ(void) {
		return acceleration[2];
	}
	double IMUData::getAngX(void) {
		return angularRate[0];
	}
	double IMUData::getAngY(void) {
		return angularRate[1];
	}
	double IMUData::getAngZ(void) {
		return angularRate[2];
	}

	int IMUData::getGPSWeek(void) {
		return GPSWeek;
	}
	double IMUData::getGPSTow(void) {
		return GPSToW;
	}

}