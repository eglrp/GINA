#pragma once

#include "std_IMUSimulator.h"
#include <iostream>
#include <vector>

// GINAParser

#include "I_GINA_writer.hpp"
#include "TrajectoryData.hpp"


namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class PositionData :public GINASimulator::I_TrajectoryData {

	public:

		PositionData(void);

		int GPSWeek;
		double GPSToW;
		double ecef[3];
		double attitude[3];

		PositionData& operator=(GINASimulator::TrajectoryData&);
		PositionData& operator=(Position_IMU&);
		Position_IMU& operator=(PositionData&);

		virtual double getECEF_X(void);
		virtual double getECEF_Y(void);
		virtual double getECEF_Z(void);
		virtual double getRoll(void);
		virtual double getPitch(void);
		virtual double getYaw(void);

		virtual int getGPSWeek(void);
		virtual double getGPSTow(void);



	private:

	};

}