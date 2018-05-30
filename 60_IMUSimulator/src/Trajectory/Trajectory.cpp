
#include "Trajectory.h"
#include "lib_Frames.h"


namespace IMUSimulator {

	Trajectory::Trajectory(void) {
	
	}
	void Trajectory::add_position(double x, double y, double z, double lat, double lon, double h, int wn, double tow, double roll, double pitch, double yaw) {

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

	void Trajectory::add_position(CoordiateFrame coorframe, double coor[], TimeFrame timeframe, int wn, double tow, double roll, double pitch, double yaw) {

		node.rollpitchyaw[0] = roll;
		node.rollpitchyaw[1] = pitch;
		node.rollpitchyaw[2] = yaw;
		this->add_position(coorframe, coor, timeframe, wn, tow);
	}

	void Trajectory::add_position(CoordiateFrame coorframe, double coor[], TimeFrame timeframe, int wn, double tow) {

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

		transform2missingCoordinateFrame(coorframe, timeframe);

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
	
		transform2missingCoordinateFrame(coorframe, timeframe);

		this->add_position();
	}

	void Trajectory::add_position(void) {
	
		traj_data.push_back(node);
		clearNode();
	}

	strapdown_ecef& operator >> (strapdown_ecef& str, Trajectory& traj) {

		Eigen::Vector3d	llh = str.getLLH_in_DEG();
		traj.add_position(	str.ecef[0], str.ecef[1], str.ecef[2],
							llh[0], llh[1], llh[2],
							0.0, 0.0,
							str.rollpitchyaw[0], str.rollpitchyaw[1], str.rollpitchyaw[2]);

		return str;
	}

	PositionData& operator >> (PositionData& pos , Trajectory& traj) {
		
		double llh[3];

		Lib::transform_ecef2llh(pos.ecef[0],
								pos.ecef[1],
								pos.ecef[2],
								llh[0],
								llh[1],
								llh[2]);

		traj.add_position(	pos.ecef[0], pos.ecef[1], pos.ecef[2],
							llh[0], llh[1], llh[2],
							pos.GPSWeek, pos.GPSToW,
							pos.attitude[0], pos.attitude[1], pos.attitude[2]);

		return pos;
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

		node.tow = 0.0;
		node.wn = 0;
	}

	void Trajectory::transform2missingCoordinateFrame(CoordiateFrame coorframe, TimeFrame timeframe) {

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