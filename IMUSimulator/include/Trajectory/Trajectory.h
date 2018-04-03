#pragma once


#include "geoparam_wgs84.h"
#include "std_IMUSimulator.h"

#include <iostream>
#include <vector>

namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class Trajectory {
		
		public:
			std::vector<Position_IMU> traj_data;
			Trajectory(void);
	
			void add_position(CoordiateFrame, double[], TimeFrame, double, double);
			void add_position(CoordiateFrame, TimeFrame, Position_IMU);
			void printf(std::ostream&);

			friend std::ostream& operator<<(std::ostream&, Trajectory&);
	};



}