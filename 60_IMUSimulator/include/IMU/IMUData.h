#pragma once

#include "std_IMUSimulator.h"

#include <iostream>
#include <vector>

// PINAParser
//#include "IMUBase.hpp"
#include "IMUData.hpp"
#include "IMUHeader.hpp"
/*#include "IMUStream.hpp"
#include "IMUStore.hpp"*/

/*#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"*/

namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class IMUData {

	public:

		int GPSWeek;
		double ToW;
		double acceleration[3];
		double angularRate[3];

		IMUData& operator=(PINASimulator::IMUData&);

	private:

	};

}