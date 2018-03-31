
#include "Trajectory.h"
#include "lib_Frames.h"


namespace IMUSimulator {

	Trajectory::Trajectory(void) {
	
	}

	void Trajectory::add_position(CoordiateFrame coorframe, double coor[], TimeFrame timeframe, double wn, double tow) {

		Position_IMU node;
		// Here we don't care about the type of the coordinates. The next add_position will decide. We just have to fill both type off coordinates with the inputs.
		node.ecef[0] = coor[0];
		node.ecef[1] = coor[1];
		node.ecef[2] = coor[2];
		node.llh[0] = coor[0];
		node.llh[1] = coor[1];
		node.llh[2] = coor[2];
		node.tow = tow;
		node.wn = wn;

		this->add_position(coorframe, timeframe, node);
	}

	void Trajectory::add_position(CoordiateFrame coorframe, TimeFrame timeframe, Position_IMU pos) {
	
		Position_IMU node;

		if (coorframe == ECEF_Frame) {

			Lib::transform_ecef2llh(pos.ecef[0], pos.ecef[1], pos.ecef[2], pos.llh[0], pos.llh[1], pos.llh[2]);
		}
		else if (coorframe == LLH_Frame) {

			Lib::transform_llh2ecef(pos.llh[0], pos.llh[1], pos.llh[2], pos.ecef[0], pos.ecef[1], pos.ecef[2]);
		}
		else {

			// throw exception
		}

		traj_data.push_back(pos);
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
}