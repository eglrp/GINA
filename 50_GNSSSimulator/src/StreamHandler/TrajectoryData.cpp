#include "TrajectoryData.hpp"

namespace gnsssimulator 
{
	using namespace gpstk::StringUtils;
	using namespace std;

	/**
	* This function retrieves a RINEX NAV record from the given FFStream.
	* If an error is encountered in reading from the stream, the stream
	* is returned to its original position and its fail-bit is set.
	* @throws StringException when a StringUtils function fails
	* @throws FFStreamError when exceptions(failbit) is set and
	*  a read or formatting error occurs.  This also resets the
	*  stream to its pre-read position.
	*/

	bool TrajectoryData::compare(const TrajectoryData& other) const{
		return (this->pos == other.pos && this->gpsTime == other.gpsTime);
	}

	//bool operator== (const Triple& right) const;
	bool TrajectoryData::operator==(const TrajectoryData& other) const{
		return this->compare(other);
	}

	bool TrajectoryData::operator!=(const TrajectoryData& other) const {
		return !this->compare(other);
	}

	TrajectoryData& TrajectoryData::operator+=(gpstk::Position& newpos) {
		this->pos = newpos; // TODO not correct. sth wrong
		this->coorSys = newpos.getCoordinateSystem();
		return *this;
	}

	TrajectoryData& TrajectoryData::operator=(gpstk::Position& newpos) {
		this->pos = newpos;
		this->coorSys = newpos.getCoordinateSystem();
		return *this;
	}

	TrajectoryData& TrajectoryData::operator=(GINAParser::TrajectoryData& trajData) {
		this->pos = trajData.pos;
		this->coorSys = trajData.coorSys;
		this->gpsTime = gpstk::GPSWeekSecond(trajData.time);
		return *this;
	
	}

}