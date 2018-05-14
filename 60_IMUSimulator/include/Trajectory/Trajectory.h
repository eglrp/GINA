#pragma once

#include "geoparam_wgs84.h"
#include "strapdown_ecef.h"
#include "PositionData.h"
#include "std_IMUSimulator.h"
#include <iostream>
#include <vector>

#include "I_PINA_writer.hpp"

namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class Trajectory {
		
		public:
			Position_IMU node;						// this is a temp variable. Used to collect every position data before we can push it to the Position_IMU vector
			std::vector<Position_IMU> traj_data;
			Trajectory(void);

			void add_position(double, double, double, double, double, double, int, double, double, double, double);
			void add_position(CoordiateFrame, double[], TimeFrame, int, double, double, double, double);
			void add_position(CoordiateFrame, double[], TimeFrame, int, double);
			void add_position(CoordiateFrame, TimeFrame, Position_IMU);

			void updateTime(TimeFrame, int, double);

			friend std::ostream& operator<<(std::ostream&, Trajectory&);
			friend std::ostream& operator<<(std::ostream&, const Position_IMU&);

			friend strapdown_ecef& operator>>(strapdown_ecef&, Trajectory&);
			friend PositionData& operator>>(PositionData&, Trajectory&);

		private:
			void printf(std::ostream&);
			void clearNode(void);
			void add_position(void);
			void add_position(CoordiateFrame, TimeFrame);
			void transform2missingCoordinateFrame(CoordiateFrame, TimeFrame);
	};
}