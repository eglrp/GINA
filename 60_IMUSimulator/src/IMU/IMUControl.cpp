
#include "IMUControl.h"


namespace IMUSimulator {

	IMUControlCommand::IMUControlCommand(void) {

		Eigen::Vector3d null;
		null << 0, 0, 0;
		this->a_contol = null;
		this->w_control = null;
		this->startWeek = 0;
		this->endWeek = 0;
		this->startGPSToW = 0;
		this->endGPSToW = 0;
		this->dt = 0;
	};

	IMUControlCommand::IMUControlCommand(	Eigen::Vector3d a_contol,
											Eigen::Vector3d w_control,
											unsigned int startWeek,
											double startGPSToW,
											unsigned int endWeek,
											double endGPSToW,
											double dt) {
	
		this->a_contol = a_contol;
		this->w_control = w_control;
		this->startWeek = startWeek;
		this->endWeek = endWeek;
		this->startGPSToW = startGPSToW;
		this->endGPSToW = endGPSToW;
		this->dt = dt;

		checkValidity();
		
	};

	unsigned char IMUControlCommand::checkValidity(void) {
	
		if (startWeek + 1 == endWeek) {
			endGPSToW = 604800;
		}
		else if (startWeek == endWeek) {
			if (startGPSToW > endGPSToW) {
				throw("Error! End time is less than start time.");
			}
		}
		else if (startWeek + 1 < endWeek) {
			throw("Error! IMUControlCommand method can not handle multiply week rollover.");
		}
		else {
			throw("Error! Endweek is less than start week.");
		}
	}

	IMUControlCommand& IMUControlCommand::operator=(GINASimulator::IMUControlData& node) {

		IMUControlCommand new_node;

		new_node.a_contol[0] = node.acceleration[0];
		new_node.a_contol[1] = node.acceleration[1];
		new_node.a_contol[2] = node.acceleration[2];

		new_node.w_control[0] = node.angularRate[0];
		new_node.w_control[1] = node.angularRate[1];
		new_node.w_control[2] = node.angularRate[2];

		new_node.startWeek = node.getStartGPSWeek();
		new_node.startGPSToW = node.getStartGPSToW();

		new_node.endWeek = node.getEndGPSWeek();
		new_node.endGPSToW = node.getEndGPSToW();

		new_node.dt = node.getTimeStep();

		return new_node;
	}

	double IMUControlCommand::getAccX(void) {
		return a_contol[0];
	}
	double IMUControlCommand::getAccY(void) {
		return a_contol[1];
	}
	double IMUControlCommand::getAccZ(void) {
		return a_contol[2];
	}
	double IMUControlCommand::getAngX(void) {
		return w_control[0];
	}
	double IMUControlCommand::getAngY(void) {
		return w_control[1];
	}
	double IMUControlCommand::getAngZ(void) {
		return w_control[2];
	}
	
	int IMUControlCommand::getStartGPSWeek(void) {
		return startWeek;
	}
	double IMUControlCommand::getStartGPSTow(void) {
		return startGPSToW;
	}

	int IMUControlCommand::getEndGPSWeek(void) {
		return endWeek;
	}
	double IMUControlCommand::getEndGPSTow(void) {
		return endGPSToW;
	}

	double IMUControlCommand::getTimeStep(void) {
		return dt;
	}

	IMUControl::IMUControl(void) { 

		Eigen::Vector3d startVelocity_body;
		startVelocity_body << 0, 0, 0;
		setIMUControl(	0, 0, 0, 0, 0, 0, startVelocity_body);
	
	};

	IMUControl::IMUControl(	double lat, double lon, double height,
							double roll, double pitch, double yaw,
							Eigen::Vector3d startVelocity_body) {
	
		setIMUControl(	lat, lon, height,
						roll, pitch, yaw,
						startVelocity_body);
	
	}

	unsigned char IMUControl::setIMUControl(double lat, double lon, double height,
											double roll, double pitch, double yaw,
											Eigen::Vector3d startVelocity_body) {
	
		Eigen::Vector3d rollpitchyaw, llh, ecef;

		llh << lat, lon, height;
		rollpitchyaw << roll, pitch, yaw;
		ecef = IMUSimulator::Lib::transform_llh2ecef(llh);

		IMUSimulator::IMUSignalGenerator imuGenerator;
		IMUSimulator::strapdown_ecef str_e(rollpitchyaw, startVelocity_body, ecef);

		this->imuGenerator = imuGenerator;
		this->str_e = str_e;
	
		return true;
	}

	unsigned char IMUControl::setCommand(IMUControlCommand imuContrCommand) {
	
		setCommand(	imuContrCommand.startWeek, imuContrCommand.startGPSToW,
					imuContrCommand.endWeek, imuContrCommand.endGPSToW,
					imuContrCommand.dt,
					imuContrCommand.a_contol,
					imuContrCommand.w_control);

		return true;
	}

	unsigned char IMUControl::setCommand(unsigned int startWeek, double startGPSToW,
											unsigned int endWeek, double endGPSToW,
											double dt,
											Eigen::Vector3d a_contol, 
											Eigen::Vector3d w_control) {
	
		isfinished			= false;
		firstStep			= true;
		this->startWeek		= startWeek;
		this->startGPSToW	= startGPSToW;
		this->endWeek		= endWeek;
		this->endGPSToW		= endGPSToW;
		this->dt			= dt;
		this->a_contol		= a_contol;
		this->w_control		= w_control;

		return true;
	}

	unsigned char IMUControl::runAll(void) {
	
		if (firstStep) {
			setFirstStep();
			firstStep = false;
		}
		else {
			throw("Error! There was no new command.");
		}

		while (time <= endTime) {

			generatetrajectory(	imuGenerator, str_e,
								posData, meas,
								a_contol, w_control,
								WN, time,
								dt);

			timeIncrease(time, dt);

			if (breakCondition()) {
				break;
			}
		}

		return true;
	}

	unsigned char IMUControl::runStep(void) {
	
		bool rtnv = true;
		if (firstStep) {
			setFirstStep();
			firstStep = false;
		}

		if (isfinished) {
			rtnv = false;
			return rtnv;
		}

		generatetrajectory(	imuGenerator, str_e,
							posData, meas,
							a_contol, w_control,
							WN, time,
							dt);

		timeIncrease(time, dt);
			
		if( breakCondition() ){
			rtnv = false;
			isfinished = true;
		}
	
		return rtnv;
	}

	bool IMUControl::getPositionData(PositionData& outData) {
	
		outData.attitude[0] = this->posData.attitude[0];
		outData.attitude[1] = this->posData.attitude[1];
		outData.attitude[2] = this->posData.attitude[2];

		outData.ecef[0] = this->posData.ecef[0];
		outData.ecef[1] = this->posData.ecef[1];
		outData.ecef[2] = this->posData.ecef[2];

		outData.GPSToW = this->posData.GPSToW;
		outData.GPSWeek = this->posData.GPSWeek;

		return true;
	}

	Measure_IMU IMUControl::getMeasurement(void) {
	
		return this->meas;
	}

	void IMUControl::generatetrajectory(IMUSimulator::IMUSignalGenerator& imuGenerator,
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


	unsigned char IMUControl::setFirstStep(void) {

		startTime = startGPSToW;
		endTime = endGPSToW;
		deltaT = dt;
		startWN = startWeek;
		endWN = endWeek;

		time = startTime;
		WN = startWN;

		errorCheck();

		if (startWN < endWN) {
			endTime = 604800;
		}

		return true;
	}

	unsigned char IMUControl::timeIncrease(double& time, double dt) {

		/*with weekrollover check*/
		if (time + dt > 604800) {
			if (WN + 1 < endWN) {
				WN += 1;
			}
			else if (WN + 1 == endWN) {
				WN += 1;
				endTime = this->endGPSToW;
			}
			time += dt;
			time -= 604800;
		}
		else {
			time += dt;
		}
		
		return true;
	}

	unsigned char IMUControl::errorCheck(void) {
	
		if (startWN == endWN) {
			if (startTime > endTime) {
				throw("Error! End time is less than start time.");
			}
			else if (startTime == endTime) {
				throw("Error! End time is the same as start time.");
			}
		}
		else if (startWN > endWN) {
			throw("Error! End week is the same as start week.");
		}
	}

	unsigned char IMUControl::breakCondition(void) {

		bool rtnv = false;

		if (WN <= endWN) {
			if (time > endTime) {
				rtnv = true;
			}
		}
		return rtnv;
	}
}