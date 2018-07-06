#pragma once
namespace GINASimulator {

	class I_IMUControlData {

	public:
		virtual double getAccX(void) = 0;
		virtual double getAccY(void) = 0;
		virtual double getAccZ(void) = 0;
		virtual double getAngX(void) = 0;
		virtual double getAngY(void) = 0;
		virtual double getAngZ(void) = 0;

		virtual int getGPSWeek(void) = 0;
		virtual double getGPSTow(void) = 0;

	};

	class I_IMUData {

	public:
		virtual double getAccX(void) = 0;
		virtual double getAccY(void) = 0;
		virtual double getAccZ(void) = 0;
		virtual double getAngX(void) = 0;
		virtual double getAngY(void) = 0;
		virtual double getAngZ(void) = 0;

		virtual int getGPSWeek(void) = 0;
		virtual double getGPSTow(void) = 0;

	};

	class I_TrajectoryData {

	public:
		virtual double getECEF_X(void) = 0;
		virtual double getECEF_Y(void) = 0;
		virtual double getECEF_Z(void) = 0;
		virtual double getRoll(void) = 0;
		virtual double getPitch(void) = 0;
		virtual double getYaw(void) = 0;

		virtual int getGPSWeek(void) = 0;
		virtual double getGPSTow(void) = 0;

	};

}