
#include "IMUControl.h"


namespace IMUSimulator {

	IMUControlCommand::IMUControlCommand(void) {

	
		this->a_contol[0] = 0;
		this->a_contol[1] = 0;
		this->a_contol[2] = 0;
		this->w_control[0] = 0;
		this->w_control[1] = 0;
		this->w_control[2] = 0;

		this->startWeek = 0;
		this->endWeek = 0;
		this->startGPSToW = 0;
		this->endGPSToW = 0;
		this->dt = 0;
	};

	IMUControlCommand::IMUControlCommand(const GINAParser::IMUControlData& data) {

		this->a_contol[0] = data.acceleration[0];
		this->a_contol[1] = data.acceleration[1];
		this->a_contol[2] = data.acceleration[2];

		this->w_control[0] = data.angularRate[0];
		this->w_control[1] = data.angularRate[1];
		this->w_control[2] = data.angularRate[2];

		this->startWeek = data.getStartGPSWeek();
		this->startGPSToW = data.getStartGPSToW();
		this->endWeek = data.getEndGPSWeek();
		this->endGPSToW = data.getEndGPSToW();

		this->dt = data.getTimeStep();

	}

	/*IMUControlCommand::IMUControlCommand(	Eigen::Vector3d a_contol,
											Eigen::Vector3d w_control,
											unsigned int startWeek,
											double startGPSToW,
											unsigned int endWeek,
											double endGPSToW,
											double dt) {
	
		this->a_contol[0] = a_contol[0];
		this->a_contol[1] = a_contol[1];
		this->a_contol[2] = a_contol[2];
		this->w_control[0] = w_control[0];
		this->w_control[1] = w_control[1];
		this->w_control[2] = w_control[2];
		this->startWeek = startWeek;
		this->endWeek = endWeek;
		this->startGPSToW = startGPSToW;
		this->endGPSToW = endGPSToW;
		this->dt = dt;

		checkValidity();
		
	};*/

	IMUControlCommand::IMUControlCommand(	double a_contol[3],
											double w_control[3],
											unsigned int startWeek,
											double startGPSToW,
											unsigned int endWeek,
											double endGPSToW,
											double dt) {

		this->a_contol[0] = a_contol[0];
		this->a_contol[1] = a_contol[1];
		this->a_contol[2] = a_contol[2];
		this->w_control[0] = w_control[0];
		this->w_control[1] = w_control[1];
		this->w_control[2] = w_control[2];
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

	IMUControlCommand& IMUControlCommand::operator=(GINAParser::IMUControlData& node) {

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

	double IMUControlCommand::getAccX(void) const {
		return a_contol[0];
	}
	double IMUControlCommand::getAccY(void) const {
		return a_contol[1];
	}
	double IMUControlCommand::getAccZ(void) const {
		return a_contol[2];
	}
	double IMUControlCommand::getAngX(void) const {
		return w_control[0];
	}
	double IMUControlCommand::getAngY(void) const {
		return w_control[1];
	}
	double IMUControlCommand::getAngZ(void) const {
		return w_control[2];
	}
	
	int IMUControlCommand::getStartGPSWeek(void) const {
		return startWeek;
	}
	double IMUControlCommand::getStartGPSTow(void) const {
		return startGPSToW;
	}

	int IMUControlCommand::getEndGPSWeek(void) const {
		return endWeek;
	}
	double IMUControlCommand::getEndGPSTow(void) const {
		return endGPSToW;
	}

	double IMUControlCommand::getTimeStep(void) const {
		return dt;
	}

	IMUControl::IMUControl(void) { 

		double startVelocity_body[3];
		startVelocity_body[0] = 0; 
		startVelocity_body[1] = 0;
		startVelocity_body[2] = 0;
		setIMUControl(	0, 0, 0, 0, 0, 0, startVelocity_body);
	
	};

	IMUControl::IMUControl(	double lat, double lon, double height,
							double roll, double pitch, double yaw,
							double startVelocity_body[3]) {
	
		setIMUControl(	lat, lon, height,
						roll, pitch, yaw,
						startVelocity_body);
	
	}

	unsigned char IMUControl::setIMUControl(double lat, double lon, double height,
											double roll, double pitch, double yaw,
											double startVelocity_body[3]) {
	
		Eigen::Vector3d rollpitchyaw, llh, ecef, Vb;

		llh << lat, lon, height;
		rollpitchyaw << roll, pitch, yaw;
		Vb << startVelocity_body[0], startVelocity_body[1], startVelocity_body[2];
		ecef = IMUSimulator::Lib::transform_llh2ecef(llh);

		IMUSimulator::IMUSignalGenerator imuGenerator;

		IMUSimulator::strapdown_ecef str_e(rollpitchyaw, Vb, ecef);

		this->imuGenerator = imuGenerator;
		this->str_e = str_e;
	
		return true;
	}

	unsigned char IMUControl::setCommand(const IMUControlCommand imuContrCommand) {
	
		setCommand(	imuContrCommand.startWeek, imuContrCommand.startGPSToW,
					imuContrCommand.endWeek, imuContrCommand.endGPSToW,
					imuContrCommand.dt,
					imuContrCommand.a_contol,
					imuContrCommand.w_control);

		return true;
	}

	unsigned char IMUControl::setCommand(	const unsigned int startWeek, const double startGPSToW,
											const unsigned int endWeek, const double endGPSToW,
											const double dt,
											const double a_control[3],
											const double w_control[3]) {
	
		isfinished			= false;
		firstStep			= true;
		this->startWeek		= startWeek;
		this->startGPSToW	= startGPSToW;
		this->endWeek		= endWeek;
		this->endGPSToW		= endGPSToW;
		this->dt			= dt;
		this->a_control[0] = a_control[0]; 
		this->a_control[1] = a_control[1];
		this->a_control[2] = a_control[2];

		this->w_control[0] = w_control[0];
		this->w_control[1] = w_control[1];
		this->w_control[2] = w_control[2];

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
								a_control, w_control,
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
							a_control, w_control,
							WN, time,
							dt);

		timeIncrease(time, dt);
			
		if( breakCondition() ){
			rtnv = false;
			isfinished = true;
		}
	
		return rtnv;
	}

	bool IMUControl::getPositionData(PositionData& outData) const {
	
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

	Measure_IMU IMUControl::getMeasurement(void) const {
	
		return this->meas;
	}

	bool IMUControl::getMeasurement(Measure_IMU& meas) const {

		meas.a[0] = this->meas.a[0];
		meas.a[1] = this->meas.a[1];
		meas.a[2] = this->meas.a[2];

		meas.w[0] = this->meas.w[0];
		meas.w[1] = this->meas.w[1];
		meas.w[2] = this->meas.w[2];

		meas.tow = this->meas.tow;
		meas.wn = this->meas.wn;

		return true;
	}

	void IMUControl::generatetrajectory(IMUSimulator::IMUSignalGenerator& imuGenerator,
										IMUSimulator::strapdown_ecef& str_e,
										IMUSimulator::PositionData& posData,
										IMUSimulator::Measure_IMU& meas,
										double ab[3],
										double wb[3],
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
	
		if (dt == 0) {
			cout << "Error. TimeStep is Zero." << endl;
			throw("Error. TimeStep is Zero.");
		}
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

		return true;
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