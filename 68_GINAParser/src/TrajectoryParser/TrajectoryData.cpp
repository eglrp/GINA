
#include "TrajectoryData.hpp"

namespace GINAParser
{
	using namespace gpstk::StringUtils;
	using namespace std;

	const string TrajectoryData::startofDataTag = "START OF DATA";


	TrajectoryData& TrajectoryData::operator=(I_TrajectoryData& data) {
	

		this->coorSys = gpstk::Position::CoordinateSystem::Cartesian;
		this->pos = gpstk::Position(data.getECEF_X(), data.getECEF_Y(), data.getECEF_Z());

		this->timeSys = gpstk::TimeSystem::Systems::GPS;
		this->time = gpstk::GPSWeekSecond(data.getGPSWeek(), data.getGPSTow());

		this->attitude[0] = data.getRoll();
		this->attitude[1] = data.getPitch();
		this->attitude[2] = data.getYaw();

		return *this;
	}

	void TrajectoryData::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (!strm.dataStartWritten) {
			strm << startofDataTag << endl;
			strm.lineNumber++;
			strm.dataStartWritten = true;
		}

		if (timeSys == gpstk::TimeSystem::Systems::GPS) {
			gpstk::GPSWeekSecond timeGPS(time);
			strm << timeGPS.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGPS.getSOW() << "  ";
		}
		else if (timeSys == gpstk::TimeSystem::Systems::GAL) {
			gpstk::GALWeekSecond timeGAL(time);
			strm << timeGAL.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGAL.getSOW() << "  ";
		}
		else {
			strm << 0.0 << "  ";
			strm << fixed << std::setprecision(5) << 0.0 << "  ";
		}

		if (coorSys == gpstk::Position::CoordinateSystem::Cartesian) {
			strm << fixed << std::setprecision(5) << pos.getX() << "  ";
			strm << fixed << std::setprecision(5) << pos.getY() << "  ";
			strm << fixed << std::setprecision(5) << pos.getZ() << "  ";
		}
		else if (coorSys == gpstk::Position::CoordinateSystem::Geodetic) {
			strm << fixed << std::setprecision(12) << pos.getGeodeticLatitude() << "  ";
			strm << fixed << std::setprecision(12) << pos.getLongitude() << "  ";
			strm << fixed << std::setprecision(5) << pos.getAltitude() << "  ";
		}
		else {
			strm << fixed << std::setprecision(12) << 0.0 << "  ";
			strm << fixed << std::setprecision(12) << 0.0 << "  ";
			strm << fixed << std::setprecision(5) << 0.0 << "  ";
		}


		if (attitude == nullptr) {
			strm << "0.0" << "  ";
			strm << "0.0" << "  ";
			strm << "0.0" << "  ";
		}
		else {
			if (abs(attitude[0]) > 1e-5) {
				strm << fixed << std::setprecision(5) << attitude[0] << "  ";
			}
			else {
				strm << fixed << std::setprecision(5) << 0.0 << "  ";
			}

			if (abs(attitude[1]) > 1e-5) {
				strm << fixed << std::setprecision(5) << attitude[1] << "  ";
			}
			else {
				strm << fixed << std::setprecision(5) << 0.0 << "  ";
			}

			if (abs(attitude[2]) > 1e-5) {
				strm << fixed << std::setprecision(5) << attitude[2] << "  ";
			}
			else {
				strm << fixed << std::setprecision(5) << 0.0 << "  ";
			}
			
		}
		
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
		this->strm = &strm;

		string line;

		if (!strm.headerRead)
			strm >> strm.header;

		if (!strm.dataStartRead) {
			while (true) {

				strm.formattedGetLine(line);
				gpstk::StringUtils::stripTrailing(line);

				if (line == startofDataTag) {
					strm.dataStartRead = true;
					break;
				}
			}
		}

		if (coorSys == gpstk::Position::CoordinateSystem::Unknown) {
			this->coorSys = strm.header.coorSys;
		}
		if (timeSys == gpstk::TimeSystem::Systems::Unknown) {
			this->timeSys = strm.header.timeSys;
		}

		

		strm.formattedGetLine(line, true);
		gpstk::StringUtils::stripTrailing(line);
		parseLine(line);
		
		return;
	}

	bool TrajectoryData::compare(const TrajectoryData& other) const{
		return (this->pos == other.pos && this->timeSys == other.timeSys);
	}

	//bool operator== (const Triple& right) const;
	bool TrajectoryData::operator==(const TrajectoryData& other) const{
		return this->compare(other);
	}

	bool TrajectoryData::operator!=(const TrajectoryData& other) const {
		return !this->compare(other);
	}


	TrajectoryData& TrajectoryData::operator+=(gpstk::Position& newpos) {
		// TODO: This is not quit right here. Shall match the coor systems.
		this->pos += newpos;
		this->coorSys = newpos.getCoordinateSystem();
		return *this;
	}

	TrajectoryData& TrajectoryData::operator=(gpstk::Position& newpos) {
		this->pos = newpos;
		this->coorSys = newpos.getCoordinateSystem();
		return *this;
	}

	TrajectoryData& TrajectoryData::operator=(TrajectoryData& trajData) {

		this->pos = trajData.pos;
		this->coorSys = trajData.coorSys;
		this->timeSys = trajData.timeSys;
		this->time = trajData.time;

		this->attitude[0] = trajData.attitude[0];
		this->attitude[1] = trajData.attitude[1];
		this->attitude[2] = trajData.attitude[2];

		return *this;
	}

	double TrajectoryData::getGPSWeek(void) {
		gpstk::GPSWeekSecond GPSTime(time);
		return GPSTime.getWeek();
	}

	double TrajectoryData::getGPSToW(void) {
		gpstk::GPSWeekSecond GPSTime(time);
		return GPSTime.getSOW();
	}
	
	void TrajectoryData::parseLine(std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			
			std::stringstream   ss(currentLine);
			string week, tow, coor1, coor2, coor3, roll, pitch, yaw;

			ss >> week >> tow >> coor1 >> coor2 >> coor3 >> roll >> pitch >> yaw;

			gpstk::GPSWeekSecond timeGPS;
			gpstk::GALWeekSecond timeGAL;

			if (timeSys == gpstk::TimeSystem::GPS) {
				timeGPS.week = stoi(week, nullptr);
				timeGPS.sow = stod(tow, nullptr);

				gpstk::CommonTime temp(timeGPS);
				time = temp;

			}
			else if (timeSys == gpstk::TimeSystem::GAL) {
				timeGAL.week = stoi(week, nullptr);
				timeGAL.sow = stod(tow, nullptr);

				gpstk::CommonTime temp(timeGAL);
				time = temp;
			}


			if (coorSys == gpstk::Position::CoordinateSystem::Cartesian) {
				double coordinates[3];

				if (coor1.empty() || coor2.empty() || coor3.empty()) {
					coordinates[0] = 0;
					coordinates[1] = 0;
					coordinates[2] = 0;
					// TODO shall throw error or sys
				}
				else {
					coordinates[0] = stod(coor1, nullptr);
					coordinates[1] = stod(coor2, nullptr);
					coordinates[2] = stod(coor3, nullptr);
				}
				pos.setECEF(coordinates);
			}
			else if (coorSys == gpstk::Position::CoordinateSystem::Geodetic) {
				double geodeticLatitude = stod(coor1, nullptr);
				double longitude = stod(coor2, nullptr);
				double height = stod(coor3, nullptr);

				pos.setGeodetic(geodeticLatitude, longitude, height);
			}

			if (roll.empty() || pitch.empty() || yaw.empty()) {
				attitude[0] = 0;
				attitude[1] = 0;
				attitude[2] = 0;
			}
			else{
				attitude[0] = stod(roll, nullptr);
				attitude[1] = stod(pitch, nullptr);
				attitude[2] = stod(yaw, nullptr);
			}
			

			
		}
		catch (std::exception &e)
		{
			gpstk::FFStreamError err("std::exception: " +
				string(e.what()));
			GPSTK_THROW(err);
		}
	}

}