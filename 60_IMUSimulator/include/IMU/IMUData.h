#pragma once

#include "std_IMUSimulator.h"

#include <iostream>
#include <vector>

// PINAParser
//#include "IMUBase.hpp"
#include "IMUData.hpp"
#include "IMUHeader.hpp"

#include "I_PINA_writer.hpp"

/*#include "IMUStream.hpp"
#include "IMUStore.hpp"*/

/*#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"*/

namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class IMUData:public PINASimulator::I_IMUData {

	public:

		IMUData(void);

		int GPSWeek;
		double GPSToW;
		double acceleration[3];
		double angularRate[3];

		IMUData& operator=(PINASimulator::IMUData&);
		IMUData& operator=(Measure_IMU&);
		Measure_IMU& IMUData::operator=(IMUData&);

		double getAccX(void);
		double getAccY(void);
		double getAccZ(void);
		double getAngX(void);
		double getAngY(void);
		double getAngZ(void);

		int getGPSWeek(void);
		double getGPSTow(void);

	private:

	};

}