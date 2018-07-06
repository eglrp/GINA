
#include "IMUControlData.hpp"

namespace GINASimulator
{
	using namespace gpstk::StringUtils;
	using namespace std;

	const string IMUControlData::startofDataTag = "START OF DATA";

	IMUControlData& IMUControlData::operator=(I_IMUControlData& data) {

		this->timeSys = gpstk::TimeSystem::Systems::GPS;
		this->startTime = gpstk::GPSWeekSecond(data.getGPSWeek(), data.getGPSTow());

		this->acceleration[0] = data.getAccX();
		this->acceleration[1] = data.getAccY();
		this->acceleration[2] = data.getAccZ();

		this->angularRate[0] = data.getAngX();
		this->angularRate[1] = data.getAngY();
		this->angularRate[2] = data.getAngZ();

		gpstk::GPSWeekSecond gpsTime;
		gpsTime.week = data.getGPSWeek();
		gpsTime.sow = data.getGPSTow();

		this->startTime = gpsTime;

		return *this;
	}

	void IMUControlData::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		IMUControlStream& strm = dynamic_cast<IMUControlStream&>(ffs);

		if (!strm.dataStartWritten) {
			strm << startofDataTag << endl;
			strm.lineNumber++;
			strm.dataStartWritten = true;
		}

		if (timeSys == gpstk::TimeSystem::Systems::GPS) {
			gpstk::GPSWeekSecond timeGPS(startTime);
			strm << timeGPS.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGPS.getSOW() << "  ";
		}
		else if (timeSys == gpstk::TimeSystem::Systems::GAL) {
			gpstk::GALWeekSecond timeGAL(startTime);
			strm << timeGAL.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGAL.getSOW() << "  ";
		}
		else {
			strm << " ??? " << " ??? ";
		}

		if (timeSys == gpstk::TimeSystem::Systems::GPS) {
			gpstk::GPSWeekSecond timeGPS(endTime);
			strm << timeGPS.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGPS.getSOW() << "  ";
		}
		else if (timeSys == gpstk::TimeSystem::Systems::GAL) {
			gpstk::GALWeekSecond timeGAL(endTime);
			strm << timeGAL.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGAL.getSOW() << "  ";
		}
		else {
			strm << " ??? " << " ??? ";
		}

		strm << fixed << std::setprecision(precision) << timeStep << "  ";

		strm << fixed << std::setprecision(precision) << acceleration[0] << "  ";
		strm << fixed << std::setprecision(precision) << acceleration[1] << "  ";
		strm << fixed << std::setprecision(precision) << acceleration[2] << "  ";
		

		if (angularRate == nullptr) {
			strm << "0.0" << "  ";
			strm << "0.0" << "  ";
			strm << "0.0" << "  ";
		}
		else {
			strm << fixed << std::setprecision(precision) << angularRate[0] << "  ";
			strm << fixed << std::setprecision(precision) << angularRate[1] << "  ";
			strm << fixed << std::setprecision(precision) << angularRate[2] << "  ";
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
	void IMUControlData::reallyGetRecord(gpstk::FFStream& ffs)
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		IMUControlStream& strm = dynamic_cast<IMUControlStream&>(ffs);
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

	bool IMUControlData::compare(const IMUControlData& other) const{

		bool rtv = true;
		if (this->acceleration[0]!= other.acceleration[0]) {
			rtv = false;
		}

		if (this->acceleration[1] != other.acceleration[1]) {
			rtv = false;
		}
		if (this->acceleration[2] != other.acceleration[2]) {
			rtv = false;
		}

		if (this->startTime != other.startTime) {
			rtv = false;
		}

		if (this->endTime != other.endTime) {
			rtv = false;
		}

		if (this->timeSys != other.timeSys) {
			rtv = false;
		}

		return rtv;
	}

	//bool operator== (const Triple& right) const;
	bool IMUControlData::operator==(const IMUControlData& other) const{
		return this->compare(other);
	}

	bool IMUControlData::operator!=(const IMUControlData& other) const {
		return !this->compare(other);
	}

	IMUControlData& IMUControlData::operator=(IMUControlData& newData) {

		this->acceleration[0] = newData.acceleration[0];
		this->acceleration[1] = newData.acceleration[1];
		this->acceleration[2] = newData.acceleration[2];

		this->angularRate[0] = newData.angularRate[0];
		this->angularRate[1] = newData.angularRate[1];
		this->angularRate[2] = newData.angularRate[2];

		return *this;
	}

	double IMUControlData::getStartGPSWeek(void) {
		gpstk::GPSWeekSecond GPSTime(startTime);
		return GPSTime.getWeek();
	}

	double IMUControlData::getStartGPSToW(void) {
		gpstk::GPSWeekSecond GPSTime(startTime);
		return GPSTime.getSOW();
	}

	double IMUControlData::getEndGPSWeek(void) {
		gpstk::GPSWeekSecond GPSTime(endTime);
		return GPSTime.getWeek();
	}

	double IMUControlData::getEndGPSToW(void) {
		gpstk::GPSWeekSecond GPSTime(endTime);
		return GPSTime.getSOW();
	}
	
	void IMUControlData::parseLine(std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			
			std::stringstream   ss(currentLine);
			string startweek, starttow, endweek, endtow, deltaT, accx, accy, accz, rollRate, pitchRate, yawRate;

			ss >> startweek >> starttow >> endweek >> endtow >> deltaT >> accx >> accy >> accz >> rollRate >> pitchRate >> yawRate;

			gpstk::GPSWeekSecond timeGPS;
			gpstk::GALWeekSecond timeGAL;

			if (timeSys == gpstk::TimeSystem::GPS) {
				timeGPS.week = stoi(startweek, nullptr);
				timeGPS.sow = stod(starttow, nullptr);

				gpstk::CommonTime temp1(timeGPS);
				startTime = temp1;

				timeGPS.week = stoi(endweek, nullptr);
				timeGPS.sow = stod(endtow, nullptr);

				gpstk::CommonTime temp2(timeGPS);
				endTime = temp2;

			}
			else if (timeSys == gpstk::TimeSystem::GAL) {
				timeGAL.week = stoi(startweek, nullptr);
				timeGAL.sow = stod(starttow, nullptr);

				gpstk::CommonTime temp1(timeGAL);
				startTime = temp1;

				timeGAL.week = stoi(endweek, nullptr);
				timeGAL.sow = stod(endtow, nullptr);

				gpstk::CommonTime temp2(timeGAL);
				endTime = temp2;
			}

			if (deltaT.empty() ) {
				timeStep = 0;
				// TODO shall throw error or sys
			}
			else {
				timeStep = stod(deltaT, nullptr);
			}

			if (accx.empty() || accy.empty() || accz.empty()) {
				acceleration[0] = 0;
				acceleration[1] = 0;
				acceleration[2] = 0;
				// TODO shall throw error or sys
			}
			else {
				acceleration[0] = stod(accx, nullptr);
				acceleration[1] = stod(accy, nullptr);
				acceleration[2] = stod(accz, nullptr);
			}
				
			if (rollRate.empty() || pitchRate.empty() || yawRate.empty()) {
				angularRate[0] = 0;
				angularRate[1] = 0;
				angularRate[2] = 0;
			}
			else{
				angularRate[0] = stod(rollRate, nullptr);
				angularRate[1] = stod(pitchRate, nullptr);
				angularRate[2] = stod(yawRate, nullptr);
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