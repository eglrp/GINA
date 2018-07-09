#pragma once

#include "std_IMUSimulator.h"
#include <iostream>
#include <vector>

// GINAParser

#include "I_GINA_writer.hpp"
#include "TrajectoryData.hpp"


namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class PositionData :public GINAParser::I_TrajectoryData {

	public:

		PositionData(void);
		PositionData(const PositionData& posData); 

		int GPSWeek;
		double GPSToW;
		double ecef[3];
		double attitude[3];

		PositionData& operator=(GINAParser::TrajectoryData&);
		PositionData& operator=(Position_IMU&);
		Position_IMU& operator=(PositionData&);
		
		virtual double getECEF_X(void) const;
		virtual double getECEF_Y(void) const;
		virtual double getECEF_Z(void) const;
		virtual double getRoll(void) const;
		virtual double getPitch(void) const;
		virtual double getYaw(void) const;

		virtual int getGPSWeek(void) const;
		virtual double getGPSTow(void) const;



	private:

	};

}