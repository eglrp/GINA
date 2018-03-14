#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>

//Own includes
#include "trajectoryReader.h"
#include "rinexreader.h"	// TODO: Remove this include
#include "satDataContainer.h"
#include "TrajectoryBase.hpp"
#include "TrajectoryData.hpp"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"
#include "TrajectoryStore.hpp"
#include "PRsolution.h"

//GPSTK includes
#include "Exception.hpp"
#include "StringUtils.hpp"
#include "GNSSconstants.hpp"

#include "singleton.hpp"
#include "expandtilde.hpp"
#include "logstream.hpp"
#include "CommandLine.hpp"

#include "CommonTime.hpp"
#include "Epoch.hpp"
#include "TimeString.hpp"
#include "GPSWeekSecond.hpp"

#include "RinexSatID.hpp"

#include "RinexObsID.hpp"
#include "Rinex3ObsStream.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"

#include "Rinex3NavBase.hpp"
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavData.hpp"
#include "Rinex3NavStream.hpp"

#include "SP3Header.hpp"
#include "SP3Data.hpp"
#include "SP3Stream.hpp"

#include "SP3EphemerisStore.hpp"
#include "Rinex3EphemerisStore.hpp"
//#include "GPSEphemerisStore.hpp"
//#include "GloEphemerisStore.hpp"

#include "TropModel.hpp"
#include "EphemerisRange.hpp"
#include "Position.hpp"

#include "PRSolution2.hpp"
#include "PRSolution.hpp"

//GPSTK ext lib includes
#include "GPSEphemerisStore.hpp"
#include "GPSEphemeris.hpp"


class Simulator
{
public:

protected:

	
private:
	//bool Test_ReProcess_Solution_Rinex(void);
	
};
