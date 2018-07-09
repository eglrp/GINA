
#include "CreateTrajectorywIMUCommand.hpp"
#include "IMUControl.h"

void IMUCommandForTrajectory(std::string trajFileNamewPath, std::string imuFileNamewPath, std::string imuCommandFileNamewPath) {


	// IMU Control file parser
	GINASimulator::IMUControlStream imuControlFileIn(imuCommandFileNamewPath.c_str());
	
	GINASimulator::IMUControlHeader imuControlHeader;
	GINASimulator::IMUControlData imuControlData;

	imuControlFileIn >> imuControlHeader;

	gpstk::GPSWeekSecond startTimeinGPSTime(imuControlHeader.startTime);
	gpstk::GPSWeekSecond endTimeinGPSTime(imuControlHeader.endTime);

	Eigen::Vector3d Vb, rollpitchyaw, ecef;
	ecef << imuControlHeader.startPosition.getX(), imuControlHeader.startPosition.getY(), imuControlHeader.startPosition.getZ();

	rollpitchyaw << imuControlHeader.startAttitude[0] / 180.0*EIGEN_PI,
					imuControlHeader.startAttitude[1] / 180.0*EIGEN_PI,
					imuControlHeader.startAttitude[2] / 180.0*EIGEN_PI;
	Vb << imuControlHeader.startVelocity[0], imuControlHeader.startVelocity[1], imuControlHeader.startVelocity[2];

	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

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

	IMUSimulator::IMUSignalGenerator imuGenerator;
	IMUSimulator::strapdown_ecef str_e(rollpitchyaw, Vb, ecef);

	setGINAParsers(	trajFileOut, trajHeader,
					imuFileOut, imuHeader,
					ecef, rollpitchyaw,
					startTimeinGPSTime.week,startTimeinGPSTime.sow,
					endTimeinGPSTime.week, endTimeinGPSTime.sow);

	trajFileOut << trajHeader;
	imuFileOut << imuHeader;

	IMUSimulator::IMUControl  imuControl;
	//IMUSimulator::IMUControlCommand imuCommand( ab,	wb, startTimeinGPSTime.week, startTimeinGPSTime.sow, endTimeinGPSTime.week, endTimeinGPSTime.sow, 0);

	imuControl.setIMUControl(	imuControlHeader.startPosition.getGeodeticLatitude(), 
								imuControlHeader.startPosition.getLongitude(), 
								imuControlHeader.startPosition.getHeight(),
								rollpitchyaw(0), rollpitchyaw(1), rollpitchyaw(2),
								imuControlHeader.startVelocity);

		/*Usage of runAll() method*/
		/*imuControl.runAll();

		imuControl.getPositionData(posData);
		
		meas = imuControl.getMeasurement();

		trajFileOut << convert2GINAcompatible(posData);
		imuFileOut << convert2GINAcompatible(meas);*/
		
		imuControlFileIn >> imuControlData;
		//imuCommand = imuControlData;
		IMUSimulator::IMUControlCommand imuCommand(imuControlData);
		imuControl.setCommand(imuCommand);

		while (imuControl.runStep()) {
		
			imuControl.getPositionData(posData);
			meas = imuControl.getMeasurement();

			trajFileOut << convert2GINAcompatible(posData);
			imuFileOut << convert2GINAcompatible(meas);
		}
		
	trajFileOut.close();
	imuFileOut.close();
	imuControlFileIn.close();

	return;
}
