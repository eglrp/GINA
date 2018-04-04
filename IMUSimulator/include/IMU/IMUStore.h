#pragma once

#include "std_IMUSimulator.h"

#include <iostream>
#include <vector>

namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class IMUStore {

		public:

			Measure_IMU node;						// this is a temp variable. Used to collect every position data before we can push it to the Position_IMU vector
			std::vector<Measure_IMU> measurement;
			IMUStore(void);

			void add_measure(double, double, double, double, double, double, double, double);
			void add_measure(TimeFrame, Measure_IMU);
		
			void updateTime(TimeFrame, int, double);

			friend std::ostream& operator<<(std::ostream&, IMUStore&);
			friend std::ostream& operator<<(std::ostream&, const Measure_IMU&);

		private:
		
			void printf(std::ostream&);
			void add_measure(void);
			void clearNode(void);
	};

}