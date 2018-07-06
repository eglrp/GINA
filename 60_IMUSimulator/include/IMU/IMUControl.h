#pragma once

#include <Eigen/Dense>
#include "IMUSignalGenerator.h"
#include "strapdown_ecef.h"

namespace IMUSimulator {

	
	class IMUControlCommand {

		friend class IMUControl;

		public:
		IMUControlCommand(void);
		IMUControlCommand(Eigen::Vector3d,Eigen::Vector3d, unsigned int, unsigned int, double, double, double);


		private:
			unsigned char checkValidity(void);

			Eigen::Vector3d a_contol;
			Eigen::Vector3d w_control;
			unsigned int startWeek;
			unsigned int endWeek;
			double startGPSToW;
			double endGPSToW;
			double dt;

			

	};

	class IMUControl {

		IMUSimulator::IMUSignalGenerator imuGenerator;
		IMUSimulator::strapdown_ecef str_e = strapdown_ecef(0,0,0);
		IMUSimulator::PositionData posData;
		IMUSimulator::Measure_IMU meas;

		Eigen::Vector3d a_contol;
		Eigen::Vector3d w_control;
		unsigned int startWeek;
		unsigned int endWeek;
		double startGPSToW;
		double endGPSToW;
		double dt;

	public:
		/*Constructor*/
		IMUControl(void);

		IMUControl(	double, double, double,
					double, double, double,
					Eigen::Vector3d,
					unsigned int,
					double,
					double);

		unsigned char setIMUControl(double, double, double,
									double, double, double,
									Eigen::Vector3d,
									unsigned int,
									double,
									double);

		unsigned char setCommand(IMUControlCommand);
		
		unsigned char setCommand(unsigned int, double, unsigned int, double, double, Eigen::Vector3d, Eigen::Vector3d);
		
		unsigned char runAll(void);

		unsigned char runStep(void);

		bool getPositionData(PositionData&);

		Measure_IMU getMeasurement(void);

	private:

		bool firstStep = false;
		bool isfinished = false;
		double time, startTime, endTime, deltaT;
		int startWN, endWN, WN;
	
		unsigned char IMUControl::setFirstStep(void);
		unsigned char IMUControl::timeIncrease(double&, double);
		unsigned char IMUControl::errorCheck(void);
		unsigned char IMUControl::breakCondition(void);

		void generatetrajectory(IMUSimulator::IMUSignalGenerator& imuGenerator,
								IMUSimulator::strapdown_ecef& str_e,
								IMUSimulator::PositionData& posData,
								IMUSimulator::Measure_IMU& meas,
								Eigen::Vector3d ab,
								Eigen::Vector3d wb,
								unsigned int gpsWeek,
								double gpsToW,
								double timeIncrement);
	};

}