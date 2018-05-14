
#include "CreateTrajectorywIMUGenerator.hpp"


void IMUGeneratorForTrajectory(std::string trajFileNamewPath, std::string imuFileNamewPath) {

	double dt = 0.1;
	double startTime = 0;
	double time = 0.0;
	double endTime = 100.0;
	unsigned int startWeek = 1956;
	unsigned int endWeek = 1956;

	Eigen::Vector3d ab, wb, Vb, ab_comp, wb_comp, Vb_comp, rollpitchyaw, ecef, llh;

	ab << 0.0, 0.0, 0;
	wb << 0, 0, 0.0;
	llh << 0, 0, 0;
	rollpitchyaw << 0.0, 0.0, 1.51;
	Vb << 1.0, 0.0, 0.0;

	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	Eigen::Matrix3d Cnb;

	IMUSimulator::IMUStore imuStore;
	IMUSimulator::Trajectory traj;
	IMUSimulator::PositionData posData;
	IMUSimulator::Measure_IMU meas;
	IMUSimulator::IMUData imu_meas;

	PINASimulator::TrajectoryStream trajFileOut(trajFileNamewPath.c_str(), std::ios::out);
	PINASimulator::TrajectoryHeader trajHeader;
	PINASimulator::TrajectoryData trajData;

	PINASimulator::IMUStream imuFileOut(imuFileNamewPath.c_str(), std::ios::out);
	PINASimulator::IMUHeader imuHeader;
	PINASimulator::IMUData imuData;

	ecef = IMUSimulator::Lib::transform_llh2ecef(llh);
	
	IMUSimulator::IMUSignalGenerator imuGenerator;
	IMUSimulator::strapdown_ecef str_e(rollpitchyaw, Vb, ecef);
	
	setPINAParsers(trajFileOut, trajHeader,
					imuFileOut, imuHeader,
					ecef, rollpitchyaw,
					startWeek, startTime,
					endWeek, endTime,
					dt);

	trajFileOut << trajHeader;
	imuFileOut << imuHeader;

	for (time = startTime; time < endTime; time += dt) {

		generatetrajectory(	imuGenerator, str_e,
							posData, meas,
							ab, wb,
							startWeek, time,
							dt);

		trajFileOut << convert2PINAcompatible(posData);
		imuFileOut << convert2PINAcompatible(meas);
	}

	trajFileOut.close();
	imuFileOut.close();

	return;

}

void setPINAParsers(const PINASimulator::TrajectoryStream& trajFileOut,
	PINASimulator::TrajectoryHeader& trajHeader,
	const PINASimulator::IMUStream& imuFileOut,
	PINASimulator::IMUHeader& imuHeader,
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