#pragma once

#include <Eigen/Dense>
#include "IMUSignalGenerator.h"
#include "strapdown_ecef.h"

#include "I_GINA_writer.hpp"
#include "IMUControlData.hpp"

namespace IMUSimulator {

	
	class IMUControlCommand:public GINASimulator::I_IMUControlData {

		friend class IMUControl;

		public:
			IMUControlCommand(void);
			IMUControlCommand(const GINASimulator::IMUControlData&);
			IMUControlCommand(double[3], double[3], unsigned int, double, unsigned int, double, double);

			IMUControlCommand& operator=(GINASimulator::IMUControlData&);
			
			double getAccX(void) const;
			double getAccY(void) const;
			double getAccZ(void) const;
			double getAngX(void) const;
			double getAngY(void) const;
			double getAngZ(void) const;

			int getStartGPSWeek(void) const;
			double getStartGPSTow(void) const;

			int getEndGPSWeek(void) const;
			double getEndGPSTow(void) const;

			double getTimeStep(void) const;

		private:
			unsigned char checkValidity(void);

			double a_contol[3];
			double w_control[3];
			unsigned int startWeek;
			unsigned int endWeek;
			double startGPSToW;
			double endGPSToW;
			double dt;

			

	};

	class IMUControl {

		IMUSimulator::IMUSignalGenerator imuGenerator;
		IMUSimulator::strapdown_ecef str_e = strapdown_ecef(0.,0.,0.);
		IMUSimulator::PositionData posData;
		IMUSimulator::Measure_IMU meas;

		double a_control[3];
		double w_control[3];
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
					double[3]);

		unsigned char setIMUControl(double, double, double,
									double, double, double,
									double[3]);

		unsigned char setCommand(const IMUControlCommand);
		
		unsigned char setCommand(const unsigned int, const double, const unsigned int, const double, const double, const double[3], const double[3]);
		
		unsigned char runAll(void);

		unsigned char runStep(void);

		bool getPositionData(PositionData&) const;

		// PositionData& IMUControl::getPositionData(void) const; TODO: This cannot be done bc during the return the copy constoructure of the PositionData will be called and the usable one already occupied by Position_IMU& operator=(PositionData&); I cannot define PositionData& operator=(PositionData&); bc if it only differs by the return type it is not enough.

		Measure_IMU getMeasurement(void) const;

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
								double ab[3],
								double wb[3],
								unsigned int gpsWeek,
								double gpsToW,
								double timeIncrement);
	};

}