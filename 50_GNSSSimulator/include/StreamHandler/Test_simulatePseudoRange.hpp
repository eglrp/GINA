#pragma once

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

//GPSTK ext lib includes
#include "GPSEphemerisStore.hpp"
#include "GPSEphemeris.hpp"

void makeSimplePseudoRange(void);