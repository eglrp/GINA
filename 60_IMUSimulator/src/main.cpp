
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "GPSWeekSecond.hpp"

#include <Eigen/Dense>

#include "strapdown_ecef.h"
#include "geoparam_wgs84.h"
#include "lib_Frames.h"

#include "Trajectory.h"
#include "IMUSignalGenerator.h"
#include "IMUStore.h"

//Own includes
#include "Trajectory.h"
#include "IMUSignalGenerator.h"
#include "IMUStore.h"
#include "IMUData.h"

#include "..\68_PINAParser\include\TrajectoryParser\TrajectoryData.hpp"

// PINAParser
#include "IMUBase.hpp"
#include "IMUData.hpp"
#include "IMUHeader.hpp"
#include "IMUStream.hpp"
#include "IMUStore.hpp"

#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"


using namespace std;

PINASimulator::TrajectoryData convert2PINAcompatible(IMUSimulator::PositionData&);
PINASimulator::IMUData convert2PINAcompatible(IMUSimulator::Measure_IMU&);

void setPINAParsers(const PINASimulator::TrajectoryStream&,
					PINASimulator::TrajectoryHeader&,
					const PINASimulator::IMUStream&,
					PINASimulator::IMUHeader&,
					const Eigen::Vector3d&,
					const Eigen::Vector3d&,
					const unsigned char&, const double&,
					const unsigned char&, const double&,
					const double&);

void generatetrajectory(IMUSimulator::IMUSignalGenerator&,
						IMUSimulator::strapdown_ecef&,
						IMUSimulator::PositionData&,
						IMUSimulator::Measure_IMU&,
						Eigen::Vector3d,
						Eigen::Vector3d,
						unsigned int,
						double,
						double);

int main(int argc, char **argv) {

	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	Eigen::Vector3d ab, wb, Vb, ab_comp, wb_comp, Vb_comp;
	Eigen::Vector3d local_angle, ecef, llh;
	
	Eigen::Matrix3d Cnb;
	
	IMUSimulator::IMUStore imuStore;
	IMUSimulator::Trajectory traj;
	IMUSimulator::PositionData posData;
	IMUSimulator::Measure_IMU meas;
	IMUSimulator::IMUData imu_meas;
	
	PINASimulator::TrajectoryStream trajFileOut("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\60_IMUSimulator\\files\\example_trajectory_out_steady_state.pina", std::ios::out);
	PINASimulator::TrajectoryHeader trajHeader;
	PINASimulator::TrajectoryData trajData;

	PINASimulator::IMUStream imuFileOut("C:\\Users\\LUS2BP\\Source\\Repos\\PINA\\60_IMUSimulator\\files\\example_imu_out_steady_state.pina", std::ios::out);
	PINASimulator::IMUHeader imuHeader;
	PINASimulator::IMUData imuData;

	double dt = 0.1;
	double startTime = 0;
	double time = 0.0;
	double endTime = 100.0;
	int startWeek = 1956;
	int endWeek = 1956;
	
	ab << 0.0, 0.0, 0;
	wb << 0, 0, 0.0;
	llh << 0, 0, 0;
	ecef = IMUSimulator::Lib::transform_llh2ecef(llh);

	IMUSimulator::IMUSignalGenerator imuGenerator;
	IMUSimulator::strapdown_ecef str_e(ecef);
	local_angle = str_e.getLocalAngle();

	setPINAParsers(	trajFileOut,	trajHeader,
					imuFileOut,		imuHeader,
					ecef,			local_angle,
					startWeek,		startTime,
					endWeek,		endTime,
					dt);

	trajFileOut << trajHeader;
	imuFileOut << imuHeader;

	for (time = startTime; time < endTime; time += dt) {

		generatetrajectory(	imuGenerator,	str_e,
							posData,		meas,
							ab,				wb,
							startWeek,		time,	
							dt);

		trajFileOut << convert2PINAcompatible(posData);
		imuFileOut << convert2PINAcompatible(meas); 
	}

	trajFileOut.close();
	imuFileOut.close();

	//getchar();

	return 0;
}


PINASimulator::TrajectoryData convert2PINAcompatible(IMUSimulator::PositionData &posData) {

	PINASimulator::TrajectoryData trajData;
	trajData = posData;

	return trajData;
}

PINASimulator::IMUData convert2PINAcompatible(IMUSimulator::Measure_IMU &meas) {

	PINASimulator::IMUData imuData;
	IMUSimulator::IMUData imu_meas;

	imu_meas = meas;
	imuData = imu_meas;

	return imuData;
}

void setPINAParsers(const PINASimulator::TrajectoryStream& trajFileOut,
					PINASimulator::TrajectoryHeader& trajHeader,
					const PINASimulator::IMUStream& imuFileOut,
					PINASimulator::IMUHeader& imuHeader,
					const Eigen::Vector3d& ecef,
					const Eigen::Vector3d& local_angle,
					const unsigned char& startWeek, const double& startTime,
					const unsigned char& endWeek, const double& endTime,
					const double& dt) {

	trajHeader.timeSys = gpstk::TimeSystem::GPS;
	trajHeader.coorSys = gpstk::Position::CoordinateSystem::Cartesian;
	imuHeader.timeSys = gpstk::TimeSystem::GPS;
	imuHeader.coorSys = gpstk::Position::CoordinateSystem::Cartesian;

	gpstk::GPSWeekSecond starttime_gpstk, endtime_gpstk;
	starttime_gpstk.week = startWeek;
	starttime_gpstk.sow = startTime;
	endtime_gpstk.week = endWeek;
	endtime_gpstk.sow = endTime;
	trajHeader.endTime = endtime_gpstk;
	trajHeader.startTime = starttime_gpstk;

	imuHeader.endTime = endtime_gpstk;
	imuHeader.startTime = starttime_gpstk;

	trajHeader.epochInterval = dt;
	imuHeader.epochInterval = dt;

	trajHeader.startAttitude[0] = local_angle(0);
	trajHeader.startAttitude[1] = local_angle(1);
	trajHeader.startAttitude[2] = local_angle(2);

	imuHeader.startAttitude[0] = local_angle(0);
	imuHeader.startAttitude[1] = local_angle(1);
	imuHeader.startAttitude[2] = local_angle(2);


	trajHeader.startVelocity[0] = 0.0;
	trajHeader.startVelocity[1] = 0.0;
	trajHeader.startVelocity[2] = 0.0;

	imuHeader.startVelocity[0] = 0.0;
	imuHeader.startVelocity[1] = 0.0;
	imuHeader.startVelocity[2] = 0.0;

	double ecefarray[3];
	ecefarray[0] = ecef(0);
	ecefarray[1] = ecef(1);
	ecefarray[2] = ecef(2);

	trajHeader.startPosition.setECEF(ecefarray);
	imuHeader.startPosition.setECEF(ecefarray);
}

void generatetrajectory(IMUSimulator::IMUSignalGenerator& imuGenerator, 
						IMUSimulator::strapdown_ecef& str_e, 
						IMUSimulator::PositionData& posData,
						IMUSimulator::Measure_IMU& meas,
						Eigen::Vector3d ab, 
						Eigen::Vector3d wb, 
						unsigned int gpsWeek, 
						double gpsToW, 
						double timeIncrement) {

	Eigen::Vector3d Vb = str_e.getVbody();
	Eigen::Vector3d llh = str_e.getLLH();
	Eigen::Vector3d local_angle = str_e.getLocalAngle();

	meas = imuGenerator.calculate(ab, wb, Vb, llh, local_angle);
	meas.wn = gpsWeek;
	meas.tow = gpsToW;

	str_e.update(meas, timeIncrement);
	str_e >> posData;
}
