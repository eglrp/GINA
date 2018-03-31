
#include "Trajectory.h"
#include "lib_Frames.h"


namespace IMUSimulator {

	Trajectory::Trajectory(void) {
	
	}

	void Trajectory::add_position(CoordiateFrame coorframe, double coor[], TimeFrame timeframe, double wn, double tow) {

		Trajectory_IMU node;

		if (coorframe == ECEF_Frame) {
			
			node.ecef[0] = coor[0];
			node.ecef[1] = coor[1];
			node.ecef[2] = coor[2];
			node.tow = tow;
			node.wn = wn;

			Lib::transform_ecef2llh(coor[0], coor[1], coor[2], node.llh[0], node.llh[1], node.llh[2]);
		}
		else if (coorframe == LLH_Frame) {
		
			node.llh[0] = coor[0];
			node.llh[1] = coor[1];
			node.llh[2] = coor[2];
			node.tow = tow;
			node.wn = wn;

			Lib::transform_llh2ecef(coor[0], coor[1], coor[2], node.ecef[0], node.ecef[1], node.ecef[2]);
		}
		else {
			// throw exception
		}

		this->traj_data.push_back(node);
	}
}