
#include "IMU_std_Lib.hpp"


void setGINAParsers(const GINAParser::TrajectoryStream& trajFileOut,
					GINAParser::TrajectoryHeader& trajHeader,
					const GINAParser::IMUStream& imuFileOut,
					GINAParser::IMUHeader& imuHeader,
					const Eigen::Vector3d& ecef,
					const Eigen::Vector3d& local_angle,
					const unsigned int& startWeek, const double& startTime,
					const unsigned int& endWeek, const double& endTime) {

	setGINAParsers(	trajFileOut, trajHeader,
					imuFileOut, imuHeader,
					ecef, local_angle,
					startWeek, startTime,
					endWeek, endTime,
					INVALID_EPOCHINTERVAL_IMU_HEADER);
}


void setGINAParsers(const GINAParser::TrajectoryStream& trajFileOut,
					GINAParser::TrajectoryHeader& trajHeader,
					const GINAParser::IMUStream& imuFileOut,
					GINAParser::IMUHeader& imuHeader,
					const Eigen::Vector3d& ecef,
					const Eigen::Vector3d& local_angle,
					const unsigned int& startWeek, const double& startTime,
					const unsigned int& endWeek, const double& endTime,
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

	if (dt != INVALID_EPOCHINTERVAL_IMU_HEADER) {
		trajHeader.epochInterval = dt;
		imuHeader.epochInterval = dt;
	}
	else {
		trajHeader.epochInterval = INVALID_EPOCHINTERVAL_IMU_HEADER;
		imuHeader.epochInterval = INVALID_EPOCHINTERVAL_TRAJ_HEADER;
	}

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
	Eigen::Vector3d llh = str_e.getLLH_in_DEG();
	Eigen::Vector3d local_angle = str_e.getLocalAngle();

	meas = imuGenerator.calculate(ab, wb, Vb, llh, local_angle);
	meas.wn = gpsWeek;
	meas.tow = gpsToW;

	str_e.update(meas, timeIncrement);
	str_e >> posData;
}

GINAParser::TrajectoryData convert2GINAcompatible(IMUSimulator::PositionData &posData) {

	GINAParser::TrajectoryData trajData;
	trajData = posData;

	return trajData;
}

GINAParser::IMUData convert2GINAcompatible(IMUSimulator::Measure_IMU &meas) {

	GINAParser::IMUData imuData;
	IMUSimulator::IMUData imu_meas;

	imu_meas = meas;
	imuData = imu_meas;

	return imuData;
}