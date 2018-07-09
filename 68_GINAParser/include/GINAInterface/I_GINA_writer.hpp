#pragma once
namespace GINAParser {

	class I_IMUControlData {

	public:
		virtual double getAccX(void) const = 0;
		virtual double getAccY(void) const = 0;
		virtual double getAccZ(void) const = 0;
		virtual double getAngX(void) const = 0;
		virtual double getAngY(void) const = 0;
		virtual double getAngZ(void) const = 0;

		virtual int getStartGPSWeek(void) const = 0;
		virtual double getStartGPSTow(void) const = 0;
		virtual int getEndGPSWeek(void) const = 0;
		virtual double getEndGPSTow(void) const = 0;

		virtual double getTimeStep(void) const = 0;

	};

	class I_IMUData {

	public:
		virtual double getAccX(void) const = 0;
		virtual double getAccY(void) const = 0;
		virtual double getAccZ(void) const = 0;
		virtual double getAngX(void) const = 0;
		virtual double getAngY(void) const = 0;
		virtual double getAngZ(void) const = 0;

		virtual int getGPSWeek(void) const = 0;
		virtual double getGPSTow(void) const = 0;

	};

	class I_TrajectoryData {

	public:
		virtual double getECEF_X(void) const = 0;
		virtual double getECEF_Y(void) const = 0;
		virtual double getECEF_Z(void) const = 0;
		virtual double getRoll(void) const = 0;
		virtual double getPitch(void) const = 0;
		virtual double getYaw(void) const = 0;

		virtual int getGPSWeek(void) const = 0;
		virtual double getGPSTow(void) const = 0;

	};

}