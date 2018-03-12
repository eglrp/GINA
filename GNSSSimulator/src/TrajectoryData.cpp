#include "stdafx.h"
#include "TrajectoryData.hpp"

namespace gnsssimulator 
{
	using namespace gpstk::StringUtils;
	using namespace std;

	void TrajectoryData::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {
		string line;
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		strm.precision(6);
		strm << fixed << setprecision(0) << gpsTime.getWeek() << "   ";
		strm << fixed << setprecision(1) << gpsTime.getSOW() << setprecision(4) << "  ";

		strm << fixed << setprecision(3) << pos.getX() << "  ";
		strm << fixed << setprecision(3) << pos.getY() << "  ";
		strm << fixed << setprecision(3) << pos.getZ() << "  ";

		strm << endl;
		strm.lineNumber++;
	}

	/**
	* This function retrieves a RINEX NAV record from the given FFStream.
	* If an error is encountered in reading from the stream, the stream
	* is returned to its original position and its fail-bit is set.
	* @throws StringException when a StringUtils function fails
	* @throws FFStreamError when exceptions(failbit) is set and
	*  a read or formatting error occurs.  This also resets the
	*  stream to its pre-read position.
	*/
	void TrajectoryData::reallyGetRecord(gpstk::FFStream& ffs)
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (!strm.headerRead)
			strm >> strm.header;

		if (coorSys == gpstk::Position::CoordinateSystem::Unknown) {
			this->coorSys = strm.header.coorSys;
		}

		string line;

		strm.formattedGetLine(line, true);

		getTime(line);
		getPosition(line);

		return;
	}

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
		this->pos = newpos;
		this->coorSys = newpos.getCoordinateSystem();
		return *this;
	}

	void TrajectoryData::getTime(const std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			gpsTime.week = asInt(currentLine.substr(0, 4));
			gpsTime.sow = asDouble(currentLine.substr(7, 7));
		
		}
		catch (std::exception &e)
		{
			gpstk::FFStreamError err("std::exception: " +
				string(e.what()));
			GPSTK_THROW(err);
		}
	}

	void TrajectoryData::getPosition(const std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			double coor1 = asDouble(currentLine.substr(15, 13));
			double coor2 = asDouble(currentLine.substr(28, 13));
			double coor3 = asDouble(currentLine.substr(41, 13));

			this->pos = gpstk::Position(	coor1,	coor2,	coor3, 	coorSys,  NULL );
			this->pos = this->pos.transformTo(this->coorSys);

		}
		catch (std::exception &e)
		{
			gpstk::FFStreamError err("std::exception: " +
				string(e.what()));
			GPSTK_THROW(err);
		}
	}

}