
#include "Trajectory.h"
#include "lib_Frames.h"


namespace IMUSimulator {

	Trajectory::Trajectory(void) {
	
	}
	void Trajectory::add_position(double x, double y, double z, double lat, double lon, double h, double wn, double tow, double roll, double pitch, double yaw) {

		node.rollpitchyaw[0] = roll;
		node.rollpitchyaw[1] = pitch;
		node.rollpitchyaw[2] = yaw;

		node.llh[0] = lat;
		node.llh[1] = lon;
		node.llh[2] = h;

		node.ecef[0] = x;
		node.ecef[1] = y;
		node.ecef[2] = z;

		node.wn = wn;
		node.tow = tow;

		this->add_position();
	}

	void Trajectory::add_position(CoordiateFrame coorframe, double coor[], TimeFrame timeframe, double wn, double tow, double roll, double pitch, double yaw) {

		node.rollpitchyaw[0] = roll;
		node.rollpitchyaw[1] = pitch;
		node.rollpitchyaw[2] = yaw;
		this->add_position(coorframe, coor, timeframe, wn, tow);
	}

	void Trajectory::add_position(CoordiateFrame coorframe, double coor[], TimeFrame timeframe, double wn, double tow) {

		// Here we don't care about the type of the coordinates. The next add_position will decide. We just have to fill both type off coordinates with the inputs.
		node.ecef[0] = coor[0];
		node.ecef[1] = coor[1];
		node.ecef[2] = coor[2];
		node.llh[0] = coor[0];
		node.llh[1] = coor[1];
		node.llh[2] = coor[2];
		node.tow = tow;
		node.wn = wn;

		this->add_position(coorframe, timeframe);
	}

	void Trajectory::add_position(CoordiateFrame coorframe, TimeFrame timeframe, Position_IMU pos) {

		

		node.ecef[0] = pos.ecef[0];
		node.ecef[1] = pos.ecef[1];
		node.ecef[2] = pos.ecef[2];
		node.llh[0] = pos.llh[0];
		node.llh[1] = pos.llh[1];
		node.llh[2] = pos.llh[2];
		
		node.tow = pos.tow;
		node.wn = pos.wn;

		transoform2missingCoordinateFrame(coorframe, timeframe);

		node.rollpitchyaw[0] = pos.rollpitchyaw[0];
		node.rollpitchyaw[1] = pos.rollpitchyaw[1];
		node.rollpitchyaw[2] = pos.rollpitchyaw[2];

		this->add_position();
	}

	void Trajectory::updateTime(TimeFrame timeframe, int wn, double tow) {

		if (timeframe == IMUSimulator::TimeFrame::GPSTime) {
			traj_data.back().tow = tow;
			traj_data.back().wn = wn;
		}
		else {
			throw("Invalid TimeFrame given");
		}
	}

	void Trajectory::add_position(CoordiateFrame coorframe, TimeFrame timeframe) {
	
		transoform2missingCoordinateFrame(coorframe, timeframe);

		this->add_position();
	}

	void Trajectory::add_position(void) {
	
		traj_data.push_back(node);
		clearNode();
	}

	Trajectory& operator<<(Trajectory& traj, IMUSimulator::strapdown_ecef& str) {

		Eigen::Vector3d& local_angle = IMUSimulator::Lib::dcm2euler(str.Cnb);

		traj.add_position(	str.ecef[0], str.ecef[1], str.ecef[2],
							str.llh[0], str.llh[1], str.llh[2],
							0.0, 0.0,
							local_angle(0), local_angle(1), local_angle(2));

		return traj;
	}

	std::ostream& operator<<(std::ostream& os, Trajectory& traj) {
		
		for (std::vector<Position_IMU>::iterator it = traj.traj_data.begin(); it != traj.traj_data.end(); ++it) {
			os << *it;
		}

		return os;
	}

	void Trajectory::printf(std::ostream& os) {
	
		for (std::vector<Position_IMU>::iterator it = traj_data.begin(); it != traj_data.end(); ++it) {
			os << *it;
		}
	}

	std::ostream& operator<<(std::ostream& os, const Position_IMU& pos) {
	
		os << "Time: " << "WN: " << pos.wn << "   ToW: " << pos.tow << std::endl;
		os << "ECEF: " << pos.ecef[0] << " " << pos.ecef[1] << " " << pos.ecef[2] << "	";
		os << "LLH: " << pos.llh[0] << " " << pos.llh[1] << " " << pos.llh[2] << std::endl;
		os << std::endl << std::endl;

		return os;
	}

	void Trajectory::clearNode(void) {

		node.ecef[0] = 0.0;
		node.ecef[1] = 0.0;
		node.ecef[2] = 0.0;
		
		node.llh[0] = 0.0;
		node.llh[1] = 0.0;
		node.llh[2] = 0.0;

		node.rollpitchyaw[0] = 0.0;
		node.rollpitchyaw[1] = 0.0;
		node.rollpitchyaw[2] = 0.0;

		node.tow = 0;
		node.wn = 0;
	}

	void Trajectory::transoform2missingCoordinateFrame(CoordiateFrame coorframe, TimeFrame timeframe) {

		if (coorframe == ECEF_Frame) {

			Lib::transform_ecef2llh(node.ecef[0], node.ecef[1], node.ecef[2], node.llh[0], node.llh[1], node.llh[2]);
		}
		else if (coorframe == LLH_Frame) {

			Lib::transform_llh2ecef(node.llh[0], node.llh[1], node.llh[2], node.ecef[0], node.ecef[1], node.ecef[2]);
		}
		else {

			// throw exception
		}
	}

}