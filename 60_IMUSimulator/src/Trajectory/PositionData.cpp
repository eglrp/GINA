
#include "PositionData.h"
#include "lib_Frames.h"

namespace IMUSimulator {

	PositionData::PositionData(void) {

	}

	

	Position_IMU& PositionData::operator=(PositionData& node) {

		Position_IMU new_node;

		new_node.ecef[0] = node.ecef[0];
		new_node.ecef[1] = node.ecef[1];
		new_node.ecef[2] = node.ecef[2];

		Lib::transform_ecef2llh(	node.ecef[0],
									node.ecef[1],
									node.ecef[2],
									new_node.llh[0],
									new_node.llh[1],
									new_node.llh[2]);
		
		new_node.rollpitchyaw[0] = node.attitude[0];
		new_node.rollpitchyaw[1] = node.attitude[1];
		new_node.rollpitchyaw[2] = node.attitude[2];

		new_node.wn = node.GPSWeek;
		new_node.tow = node.GPSToW;

		return new_node;
	}

	/*PositionData & PositionData::operator=(strapdown_ecef  str)
	{
		
		PositionData pos;
		// TODO: insert return statement here
		return pos;
	}*/

	PositionData& PositionData::operator=(Position_IMU& node) {
	
		PositionData new_node;

		new_node.ecef[0] = node.ecef[0];
		new_node.ecef[1] = node.ecef[1];
		new_node.ecef[2] = node.ecef[2];

		new_node.attitude[0] = node.rollpitchyaw[0];
		new_node.attitude[1] = node.rollpitchyaw[1];
		new_node.attitude[2] = node.rollpitchyaw[2];

		new_node.GPSWeek = node.wn;
		new_node.GPSToW = node.tow;

		return new_node;
	}

	PositionData& PositionData::operator=(PINASimulator::TrajectoryData& node) {

		PositionData new_node;

		new_node.ecef[0] = node.pos.getX();
		new_node.ecef[1] = node.pos.getY();
		new_node.ecef[2] = node.pos.getZ();

		new_node.attitude[0] = node.attitude[0];
		new_node.attitude[1] = node.attitude[1];
		new_node.attitude[2] = node.attitude[2];

		new_node.GPSWeek = node.getGPSWeek();
		new_node.GPSToW = node.getGPSToW();

		return new_node;
	}

	double PositionData::getECEF_X(void) {
		return ecef[0];
	}
	double PositionData::getECEF_Y(void) {
		return ecef[1];
	}
	double PositionData::getECEF_Z(void) {
		return ecef[2];
	}
	double PositionData::getRoll(void) {
		return attitude[0];
	}
	double PositionData::getPitch(void) {
		return attitude[1];
	}
	double PositionData::getYaw(void) {
		return attitude[2];
	}

	int PositionData::getGPSWeek(void) {
		return GPSWeek;
	}
	double PositionData::getGPSTow(void) {
		return GPSToW;
	}

}