
#include "CreateTrajectorywIMUCommand.hpp"


void IMUCommandForTrajectory(std::string trajFileNamewPath, std::string imuFileNamewPath) {

	double dt = 0.1;
	double startTime = 413329;
	double time;
	double endTime = 413329 + 150;
	unsigned int startWeek = 1992;
	unsigned int endWeek = 1992;

	Eigen::Vector3d ab, wb, Vb, ab_comp, wb_comp, Vb_comp, rollpitchyaw, ecef, llh;

	ab << 0.0, 0.0, 0;
	wb << 0, 0, 0.05;
	llh << 47.464405,
		19.154166,
		120;
	// Bosch coordinates
	// 47.464405, 19.154166

	rollpitchyaw << 0.0 / 180.0*EIGEN_PI,
		0.0 / 180.0*EIGEN_PI,
		0.0 / 180.0*EIGEN_PI;
	Vb << 1., 0., 0.;

	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	Eigen::Matrix3d Cnb;

	IMUSimulator::IMUStore imuStore;
	IMUSimulator::Trajectory traj;
	IMUSimulator::PositionData posData;
	IMUSimulator::Measure_IMU meas;
	IMUSimulator::IMUData imu_meas;

	GINASimulator::TrajectoryStream trajFileOut(trajFileNamewPath.c_str(), std::ios::out);
	GINASimulator::TrajectoryHeader trajHeader;
	GINASimulator::TrajectoryData trajData;

	GINASimulator::IMUStream imuFileOut(imuFileNamewPath.c_str(), std::ios::out);
	GINASimulator::IMUHeader imuHeader;
	GINASimulator::IMUData imuData;

	ecef = IMUSimulator::Lib::transform_llh2ecef(llh);

	IMUSimulator::IMUSignalGenerator imuGenerator;
	IMUSimulator::strapdown_ecef str_e(rollpitchyaw, Vb, ecef);

	setGINAParsers(trajFileOut, trajHeader,
		imuFileOut, imuHeader,
		ecef, rollpitchyaw,
		startWeek, startTime,
		endWeek, endTime,
		dt);

	trajFileOut << trajHeader;
	imuFileOut << imuHeader;

	for (time = startTime; time < endTime; time += dt) {

		generatetrajectory(imuGenerator, str_e,
			posData, meas,
			ab, wb,
			startWeek, time,
			dt);

		trajFileOut << convert2GINAcompatible(posData);
		imuFileOut << convert2GINAcompatible(meas);
	}

	trajFileOut.close();
	imuFileOut.close();

	return;
}
