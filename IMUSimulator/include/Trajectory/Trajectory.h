#pragma once


#include "geoparam_wgs84.h"
#include "strapdown_ECEF.h"
#include "std_IMUSimulator.h"
#include <iostream>
#include <vector>

namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class Trajectory {
		
		public:
			Position_IMU node;						// this is a temp variable. Used to collect every position data before we can push it to the Position_IMU vector
			std::vector<Position_IMU> traj_data;
			Trajectory(void);

			void add_position(double, double, double, double, double, double, double, double, double, double, double);
			void add_position(CoordiateFrame, double[], TimeFrame, double, double, double, double, double);
			void add_position(CoordiateFrame, double[], TimeFrame, double, double);
			void add_position(CoordiateFrame, TimeFrame, Position_IMU);
			void printf(std::ostream&);

			void updateTime(TimeFrame, int, double);

			friend std::ostream& operator<<(std::ostream&, Trajectory&);

			friend Trajectory& operator<<(Trajectory&, strapdown_ecef&);

		private:
			void clearNode(void);
			void add_position(void);
			void Trajectory::add_position(CoordiateFrame, TimeFrame);
			void Trajectory::transoform2missingCoordinateFrame(CoordiateFrame, TimeFrame);

	};

}