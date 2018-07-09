#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "GPSWeekSecond.hpp"

#include <Eigen/Dense>

#include "strapdown_ecef.h"
#include "geoparam_wgs84.h"
#include "lib_Frames.h"

#include "Trajectory.h"
#include "IMUSignalGenerator.h"
#include "IMUStore.h"

//Own includes
#include "Trajectory.h"
#include "IMUSignalGenerator.h"
#include "IMUStore.h"
#include "IMUData.h"

#include "..\68_GINAParser\include\TrajectoryParser\TrajectoryData.hpp"

// GINAParser
#include "IMUBase.hpp"
#include "IMUData.hpp"
#include "IMUHeader.hpp"
#include "IMUStream.hpp"
#include "IMUStore.hpp"

#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"

void setGINAParsers(const GINAParser::TrajectoryStream&,
					GINAParser::TrajectoryHeader&,
					const GINAParser::IMUStream&,
					GINAParser::IMUHeader&,
					const Eigen::Vector3d&,
					const Eigen::Vector3d&,
					const unsigned int&, const double&,
					const unsigned int&, const double&);

void setGINAParsers(const GINAParser::TrajectoryStream&,
					GINAParser::TrajectoryHeader&,
					const GINAParser::IMUStream&,
					GINAParser::IMUHeader&,
					const Eigen::Vector3d&,
					const Eigen::Vector3d&,
					const unsigned int&, const double&,
					const unsigned int&, const double&,
					const double&);

void generatetrajectory(IMUSimulator::IMUSignalGenerator&,
						IMUSimulator::strapdown_ecef&,
						IMUSimulator::PositionData&,
						IMUSimulator::Measure_IMU&,
						Eigen::Vector3d,
						Eigen::Vector3d,
						unsigned int,
						double,
						double);

GINAParser::TrajectoryData convert2GINAcompatible(IMUSimulator::PositionData&);
GINAParser::IMUData convert2GINAcompatible(IMUSimulator::Measure_IMU&);