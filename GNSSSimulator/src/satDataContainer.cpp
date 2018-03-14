#include "stdafx.h"
#include "satDataContainer.h"

satDataContainer::satDataContainer()
{
}

satDataContainer::~satDataContainer()
{
}



void satDataContainer::assemblePseudoRangeContainer(SatID inSat, CivilTime inTime, double inPRange)
{
	pseudoRangeContainer[inSat][inTime] = inPRange;

}

void satDataContainer::passEphemerisStore(GPSEphemerisStore &eph)
{
	ephemerisStore = eph;
}


bool satDataContainer::isEpochonDarkSide(CivilTime civiliantime, std::vector<CivilTime>& referenceEpoch)
{
	if (civiliantime < referenceEpoch.front() || civiliantime > referenceEpoch.back())
		return true;
	else
		return false;
}

std::vector<CivilTime> satDataContainer::getEpochVectorforSat(SatID& querySat)
{
	std::map<CivilTime,double> queryMap = pseudoRangeContainer.at(querySat);
	std::vector<CivilTime> outVector;

	for (auto& x : queryMap)
		outVector.push_back(x.first);
	
	return outVector;
}

std::vector<SatID> satDataContainer::getSatIDvectorlist(void)
{
	std::vector<SatID> return_vec; // TODO
	for (auto& it : pseudoRangeContainer) {
		return_vec.push_back(it.first);
	}
	return return_vec;
}

GPSEphemerisStore & satDataContainer::getEphemerisStore()
{
	return ephemerisStore;
}

SatID satDataContainer::getSatIDObject(int i, SatID::SatelliteSystem sys = SatID::SatelliteSystem::systemGPS)
{
	//SV's are ordered
	PseudoRangeContainer::const_iterator it = pseudoRangeContainer.begin();
	if (i > 1) {
		std::advance(it, i - 1);
	}
	SatID querysat;

	if ((*it).first.system != sys)
		return (*it).first;			// TODO: return invalid SatID
	return (*it).first;
}



CivilTime satDataContainer::getCivilTimeObject(int yr, int mo, int da, int hr, int min, double sec)
{
	CivilTime returnTime;
	returnTime.setTimeSystem(TimeSystem::GPS);
	returnTime.year = yr;
	returnTime.month = mo;
	returnTime.day = da;
	returnTime.hour = hr;
	returnTime.minute = min;
	returnTime.second = sec;

	return returnTime;
}

OrbitEph satDataContainer::getSatInfoAtEpoch(SatID& query_sat, CivilTime& query_time)
{
	if (isEpochonDarkSide(query_time, getEpochVectorforSat(query_sat)))
	{
		throw "Sat has no observations at given time. Dark Side Error.";
		return OrbitEph();
	}
	// TODO: Rewrite to utilize svXVT() function instead of getting Ephemeris for the epoch.
	GPSEphemeris ephemeris = ephemerisStore.findEphemeris(query_sat,query_time);
	//returnEph = ephemerisStore.findEphemeris(query_sat, query_time);
	return ephemeris;
}

double satDataContainer::getPseudorangeatEpoch(SatID &querysat, CivilTime &querytime)
{
	double return_range;
	return_range = pseudoRangeContainer.at(querysat).at(querytime);

	return return_range;
}

PseudoRangeContainer satDataContainer::getPseudorangeContainer()
{
	return pseudoRangeContainer;
}

