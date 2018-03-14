#include "stdafx.h"
#include "TrajectoryStore.hpp"

namespace gnsssimulator {
	TrajectoryStore::TrajectoryStore()
	{
	}


	TrajectoryStore::~TrajectoryStore()
	{
	}

	void TrajectoryStore::setCorrdinateSystem(gpstk::Position::CoordinateSystem coor) {
		if (isCoorSystemSet == false && coor != gpstk::Position::CoordinateSystem::Unknown) {
			coorSys = coor;
			isCoorSystemSet = true;
		}
	}

	TrajectoryData& TrajectoryStore::addPosition( TrajectoryData data) {
		TrajectoryData ret;
		if (TrajStore.find(data.gpsTime) == TrajStore.end()) {
			setCorrdinateSystem(data.coorSys);
			if (isCoorSystemSet && coorSys != gpstk::Position::CoordinateSystem::Unknown) {
				ret = data;
				ret.pos = data.pos.transformTo(data.coorSys);
				TrajStore[data.gpsTime] = ret;
			}
		}
	    return ret;
	}

	TrajectoryData TrajectoryStore::findPosition(gpstk::GPSWeekSecond time) {
		TrajectoryData trajData;
		TrajectoryData ret;
		TrajectoryMap::iterator it;
		it = TrajStore.find(time);
		if (it != TrajStore.end()) {
			ret = it->second;
		}
		else {
			bool isTimeFound = false;
			vector<gpstk::GPSWeekSecond> timeVector = this->listTime();
			gpstk::CommonTime comtime; 
			gpstk::CommonTime original_comtime = time.convertToCommonTime();
			std::vector<gpstk::GPSWeekSecond>::iterator it_previous;
			
			gpstk::CommonTime comtime_last_valid;
			for (std::vector<gpstk::GPSWeekSecond>::iterator it = timeVector.begin(); it != timeVector.end(); it) {
				
				it_previous = it;
				gpstk::CommonTime comtime(*it);
				gpstk::CommonTime comtime_previous(*it_previous);
				comtime_last_valid = comtime;
				if ( comtime_previous <= original_comtime && original_comtime <= comtime) {
					ret = this->findPosition(comtime);
					bool isTimeFound = true;
				}

				++it;
			}
			if (isTimeFound == false) {
				ret = this->findPosition(comtime_last_valid);
			}
		}
		return ret;
	}

	bool TrajectoryStore::operator==(const TrajectoryStore& other) const {
		return this->compare(other);
	}
	bool TrajectoryStore::operator!=(const TrajectoryStore& other) const {
		return !(this->compare(other));
	}

	bool TrajectoryStore::compare(const TrajectoryStore& other) const {
		bool returnValue = true;
		TrajectoryMap::const_iterator itother;
		TrajectoryMap::const_iterator it ;

		for (it = this->TrajStore.begin(); it != this->TrajStore.end(); ++it) {
			itother = other.TrajStore.find(it->first);
			if (itother == other.TrajStore.end() || it->second != itother->second ) {
				returnValue = false;
			}
		}
		return returnValue;
	}

	vector<gpstk::GPSWeekSecond> TrajectoryStore::listTime(void) {

		vector<gpstk::GPSWeekSecond> timeVector;
		for (TrajectoryMap::const_iterator it = this->TrajStore.begin(); it != this->TrajStore.end(); ++it) {
			timeVector.push_back(it->first);
		}
		return timeVector;
	}



}
