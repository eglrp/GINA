#pragma once

#include "std_IMUSimulator.h"

#include <iostream>
#include <vector>

// GINAParser
#include "IMUData.hpp"
#include "IMUHeader.hpp"

#include "I_GINA_writer.hpp"



namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class IMUData:public GINAParser::I_IMUData {

	public:

		IMUData(void);

		int GPSWeek;
		double GPSToW;
		double acceleration[3];
		double angularRate[3];

		IMUData& operator=(GINAParser::IMUData&);
		IMUData& operator=(Measure_IMU&);
		Measure_IMU& IMUData::operator=(IMUData&);

		double getAccX(void) const;
		double getAccY(void) const;
		double getAccZ(void) const;
		double getAngX(void) const;
		double getAngY(void) const;
		double getAngZ(void) const;

		int getGPSWeek(void) const;
		double getGPSTow(void) const;

	private:

	};

}