
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"

namespace PINASimulator {

	const int TrajectoryHeader::numberofLineinHeader = 21;
	const string TrajectoryHeader::startofHeaderPinaTag = "START OF HEADER";
	const string TrajectoryHeader::secondLineOfPINATrajectoryTag = "TYPE PINA TRAJECTORY";
	const string TrajectoryHeader::creatorOfFileTag = "CREATOR";
	const string TrajectoryHeader::timeofCreationTag = "TIME OF CREATIONS";
	const string TrajectoryHeader::positionTypeLLHTag = "POSITION FORMAT LLH";
	const string TrajectoryHeader::positionTypeECEFTag = "POSITION FORMAT ECEF";
	const string TrajectoryHeader::timeSystemDefinitionTag = "TIME SYSTEM";
	const string TrajectoryHeader::startTimeTag = "START TIME";
	const string TrajectoryHeader::endTimeTag = "END TIME";
	const string TrajectoryHeader::startPositionTag = "START POSITION";
	const string TrajectoryHeader::startVelocityTag = "START VELOCITY";
	const string TrajectoryHeader::startAttitudeTag = "START ATTITUDE";
	const string TrajectoryHeader::EpochIntervalTag = "EPOCH INTERVAL";
	const string TrajectoryHeader::endOfHeaderTag = "END OF HEADER";

	void TrajectoryHeader::reallyGetRecord(gpstk::FFStream& ffs)
		throw(exception,
			gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		// if already read, just return
		if (strm.headerRead == true)
			return;

		while (valid && !isHeaderEnd) {

			string line;
			strm.formattedGetLine(line);
			gpstk::StringUtils::stripTrailing(line);

			if (line.length() == 0) continue;

			// Parse File
			if (hasStartofHeaderFound(line, ffs)) continue;

			else if (hasCreatorFound(line)) continue;
			else if (hasTimeofCreationFound(line)) continue;

			else if (hasTimeSystemFound(line)) continue;
			else if (hasStartTimeFound(line)) continue;
			else if (hasEndTimeFound(line)) continue;

			else if (hasCoordinateSystemFound(line)) continue;
			else if (hasStartPositionFound(line)) continue;
			else if (hasStartVelocityFound(line)) continue;
			else if (hasStartAttitudeFound(line)) continue;

			else if (hasEpochIntervalFound(line)) continue;

			else if (hasEndofHeaderFound(line, ffs)) continue;
			else continue;

		}
	}

	void TrajectoryHeader::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(exception, gpstk::FFStreamError, gpstk::StringUtils::StringException) {
		
		const TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		while (strm.lineNumber < numberofLineinHeader) {

			writeStartofHeader(ffs);
			writeCreatorFound(ffs);
			writeTimeofCreationFound(ffs);
			writeTimeSystemFound(ffs);
			writeStartTimeFound(ffs);
			writeEndTimeFound(ffs);
			writeCoordinateSystemFound(ffs);
			writeStartPositionFound(ffs);
			writeStartVelocityFound(ffs);
			writeStartAttitudeFound(ffs);
			writeEpochIntervalFound(ffs);
			writeEndofHeaderFound(ffs);

			writeEmptyLine(ffs);
		}
		

		return;
	}
		
	void TrajectoryHeader::dump(ostream& s) const {

	}

	/*Read methods*/
	bool TrajectoryHeader::hasStartofHeaderFound(string& line, gpstk::FFStream& ffs) {

		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);
		bool rtnv = false;
		
		if (firstLineisRead == false) {
			string line_second;
			strm.formattedGetLine(line_second);
			gpstk::StringUtils::stripTrailing(line_second);

			if (valid == false && line != startofHeaderPinaTag && line_second != secondLineOfPINATrajectoryTag) {
				valid = false;
			}

			if (line == startofHeaderPinaTag && line_second == secondLineOfPINATrajectoryTag)
			{
				valid = true;
				strm.header = *this;
				firstLineisRead = true;
			}

			rtnv = true;
		}
		else {
			rtnv = false;
		}

		return rtnv;
	}

	bool TrajectoryHeader::hasStartTimeFound(string &line) {

		if (line.find(startTimeTag) != string::npos) {
			std::stringstream   ss(line);
			string first, second, third, fourth;

			ss >> first >> second >> third >> fourth;

			gpstk::GPSWeekSecond startTimeGPS;
			gpstk::GPSWeekSecond endTimeGPS;
			gpstk::GALWeekSecond startTimeGAL;
			gpstk::GALWeekSecond endTimeGAL;

			if (startTime.getTimeSystem() == gpstk::TimeSystem::GPS) {
				startTimeGPS.week = stoi(third, nullptr);
				startTimeGPS.sow = stod(fourth, nullptr);

				gpstk::CommonTime temp(startTimeGPS);
				startTime = temp;

			}
			else if (startTime.getTimeSystem() == gpstk::TimeSystem::GAL) {
				startTimeGAL.week = stoi(third, nullptr);
				startTimeGAL.sow = stod(fourth, nullptr);

				gpstk::CommonTime temp(startTimeGAL);
				startTime = temp;
			}

			return true;
		}
		else {

			return false;
		}
	}

	bool TrajectoryHeader::hasEndTimeFound(string &line) {

		if (line.find(endTimeTag) != string::npos) {
			std::stringstream   ss(line);
			string first, second, third, fourth;

			ss >> first >> second >> third >> fourth;

			gpstk::GPSWeekSecond startTimeGPS;
			gpstk::GPSWeekSecond endTimeGPS;
			gpstk::GALWeekSecond startTimeGAL;
			gpstk::GALWeekSecond endTimeGAL;

			if (endTime.getTimeSystem() == gpstk::TimeSystem::GPS) {
				endTimeGPS.week = stoi(third, nullptr);
				endTimeGPS.sow = stod(fourth, nullptr);

				gpstk::CommonTime temp(endTimeGPS);
				endTime = temp;
			}
			else if (endTime.getTimeSystem() == gpstk::TimeSystem::GAL) {
				endTimeGAL.week = stoi(third, nullptr);
				endTimeGAL.sow = stod(fourth, nullptr);

				gpstk::CommonTime temp(endTimeGAL);
				endTime = temp;
			}

			return true;
		}
		else {
			return false;
		}

	}

	bool TrajectoryHeader::hasCoordinateSystemFound(string& line) {

		if (!isPosFormatSet && line.find(positionTypeLLHTag) != string::npos) {
			coorSys = gpstk::Position::CoordinateSystem::Geodetic;
			isPosFormatSet = true;

			return true;
		}
		else if (!isPosFormatSet && line.find(positionTypeECEFTag) != string::npos) {
			coorSys = gpstk::Position::CoordinateSystem::Cartesian;
			isPosFormatSet = true;

			return true;
		}
		else {
			return false;
		}

	}

	bool TrajectoryHeader::hasTimeSystemFound(string& line) {

		if (line.find(timeSystemDefinitionTag) != string::npos) {
			if (line.find("GPS") != string::npos) {
				timeSys.setTimeSystem(gpstk::TimeSystem::Systems::GPS);
				startTime.setTimeSystem(gpstk::TimeSystem::GPS);
				endTime.setTimeSystem(gpstk::TimeSystem::GPS);

				return true;
			}
			else if (line.find("GALILEO") != string::npos) {
				timeSys.setTimeSystem(gpstk::TimeSystem::Systems::GAL);
				startTime.setTimeSystem(gpstk::TimeSystem::GAL);
				endTime.setTimeSystem(gpstk::TimeSystem::GAL);
				return true;
			}
			else {
				timeSys.setTimeSystem(gpstk::TimeSystem::Systems::Unknown);
				//TODO: throw an error or handle this case somehow
				return true;
			}
		}
		else {
			return false;
		}

	}

	bool TrajectoryHeader::hasStartPositionFound(string& line) {

		if (isPosFormatSet && line.find(startPositionTag) != string::npos) {
			std::stringstream   ss(line);
			string first, second, third, fourth, fifth;

			ss >> first >> second >> third >> fourth >> fifth;

			if (coorSys == gpstk::Position::CoordinateSystem::Cartesian) {
				double ecef[3];

				if (third.empty() || fourth.empty() || fifth.empty()) {
					ecef[0] = 0;
					ecef[1] = 0;
					ecef[2] = 0;
					// TODO shall throw error or sys
				}
				else {
					ecef[0] = stod(third, nullptr);
					ecef[1] = stod(fourth, nullptr);
					ecef[2] = stod(fifth, nullptr);
				}
				Coordinate.setECEF(ecef);
			}
			else if (coorSys == gpstk::Position::CoordinateSystem::Geodetic) {
				double geodeticLatitude = stod(third, nullptr);
				double longitude = stod(fourth, nullptr);
				double height = stod(fifth, nullptr);

				Coordinate.setGeodetic(geodeticLatitude, longitude, height);
			}

			return true;
		}

		else {
			return false;
		}

	}

	bool TrajectoryHeader::hasStartVelocityFound(string& line) {
		if (line.find(startVelocityTag) != string::npos) {
			std::stringstream   ss(line);
			string first, second, third, fourth, fifth;

			ss >> first >> second >> third >> fourth >> fifth;


			if (third.empty() || fourth.empty() || fifth.empty()) {
				startVelocity[0] = 0;
				startVelocity[1] = 0;
				startVelocity[2] = 0;
				// TODO shall throw error or sys
			}
			else {
				startVelocity[0] = stod(third, nullptr);
				startVelocity[1] = stod(fourth, nullptr);
				startVelocity[2] = stod(fifth, nullptr);
			}
				

			return true;
		}
		else {
			return false;
		}
	}

	bool TrajectoryHeader::hasStartAttitudeFound(string& line) {
	
		if (line.find(startAttitudeTag) != string::npos) {
			std::stringstream   ss(line);
			string first, second, third, fourth, fifth;

			ss >> first >> second >> third >> fourth >> fifth;

			if (third.empty() || fourth.empty() || fifth.empty()) {
				startAttitude[0] = 0;
				startAttitude[1] = 0;
				startAttitude[2] = 0;
				// TODO shall throw error or sys
			}
			else {
				startAttitude[0] = stod(third, nullptr);
				startAttitude[1] = stod(fourth, nullptr);
				startAttitude[2] = stod(fifth, nullptr);
			}

			return true;
		}
		else {
			return false;
		}
	}

	bool TrajectoryHeader::hasEpochIntervalFound(string& line) {
		if (line.find(EpochIntervalTag) != string::npos) {
			std::stringstream   ss(line);
			string first, second, third;

			ss >> first >> second >> third;

			if (third.empty()) {
				epochInterval = 0;
				// TODO shall throw error or sys
			}
			else {
				epochInterval = stod(third, nullptr);
			}

			return true;
		}
		else {
			return false;
		}
	}

	bool TrajectoryHeader::hasEndofHeaderFound(string& line, gpstk::FFStream& ffs) {

		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs); 

		if (line == endOfHeaderTag) {
			isHeaderEnd = true;
			strm.headerRead = true;
			strm.header = *this;

			return true;
		}
		else {
			return false;
		}
	}

	bool TrajectoryHeader::hasCreatorFound(string& line) {
		if (line.find(creatorOfFileTag) != string::npos) {
			
			std::size_t index = line.find("#");
			if (index > 0) {
				Creator = line.substr(creatorOfFileTag.length() + 1, index - creatorOfFileTag.length()-1);
			}
			else {
				Creator = line.substr(creatorOfFileTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(Creator, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(Creator, std::string(1, '\t'), std::string::npos);

			return true;
		}
		else {
			return false;
		}
	
	}

	bool TrajectoryHeader::hasTimeofCreationFound(string& line) {
		if (line.find(timeofCreationTag) != string::npos) {

			std::size_t index = line.find("#");
			if (index > 0) {
				timeOfCreation = line.substr(timeofCreationTag.length() + 1, index - timeofCreationTag.length()-1);
			}
			else {
				timeOfCreation = line.substr(timeofCreationTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(timeOfCreation, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(timeOfCreation, std::string(1, '\t'), std::string::npos);

			return true;
		}
		else {
			return false;
		}
	}

	/*Write methods*/
	bool TrajectoryHeader::writeStartofHeader( gpstk::FFStream& ffs) const {
	
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);
		if (strm.lineNumber == 0) {

			strm << startofHeaderPinaTag << endl;
			strm.lineNumber++;
			return true;
		}
		else if (strm.lineNumber == 1){

			strm << secondLineOfPINATrajectoryTag << endl;
			strm.lineNumber++;
			return true;
		}
		else {

			return false;
		}
		return false;
	}

	bool TrajectoryHeader::writeCreatorFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 4) {
			strm << creatorOfFileTag << " " << Creator << endl;
			strm.lineNumber++;
			return true;
		}
		
		return false;
	}

	bool TrajectoryHeader::writeTimeofCreationFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 5) {
			strm << timeofCreationTag << " " << timeOfCreation << endl;
			strm.lineNumber++;

			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeTimeSystemFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 8) {

			if (timeSys.getTimeSystem() == gpstk::TimeSystem::Systems::GPS) {
				strm << timeSystemDefinitionTag << " " << "GPS" << endl;
			}
			else if (timeSys.getTimeSystem() == gpstk::TimeSystem::Systems::GAL) {
				strm << timeSystemDefinitionTag << " " << "GAL" << endl;
			}
			else {
				strm << timeSystemDefinitionTag << " " << timeSys.getTimeSystem() << endl;
			}
			strm.lineNumber++;

			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeStartTimeFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 9) {

			if (timeSys.getTimeSystem() == gpstk::TimeSystem::Systems::GPS) {
				gpstk::GPSWeekSecond startGPSTime(startTime);
				strm << startTimeTag << " " << startGPSTime.week << " " << fixed << std::setprecision(5) << startGPSTime.sow << endl;
			}
			else if (timeSys.getTimeSystem() == gpstk::TimeSystem::Systems::GAL) {
				gpstk::GALWeekSecond startGALTime(startTime);
				strm << startTimeTag << " " << startGALTime.week << " " << fixed << std::setprecision(5) << startGALTime.sow << endl;
			}
			else {
				strm << startTimeTag << " "  << endl;
			}
			strm.lineNumber++;


			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeEndTimeFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 10) {

			if (timeSys.getTimeSystem() == gpstk::TimeSystem::Systems::GPS) {
				gpstk::GPSWeekSecond startGPSTime(endTime);
				strm << endTimeTag << " " << startGPSTime.week << " " << fixed << std::setprecision(5) << startGPSTime.sow << endl;
			}
			else if (timeSys.getTimeSystem() == gpstk::TimeSystem::Systems::GAL) {
				gpstk::GALWeekSecond startGALTime(endTime);
				strm << endTimeTag << " " << startGALTime.week << " " << fixed << std::setprecision(5) << startGALTime.sow << endl;
			}
			else {
				strm << endTimeTag << " "  << endl;
			}
			strm.lineNumber++;

			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeCoordinateSystemFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 12) {

			if (coorSys == gpstk::Position::CoordinateSystem::Cartesian) {
				strm << positionTypeECEFTag << endl;
				strm.lineNumber++;
			}
			else if (coorSys == gpstk::Position::CoordinateSystem::Geodetic) {
				strm << positionTypeLLHTag << endl;
				strm.lineNumber++;
			}

			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeStartPositionFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 14) {

			if (coorSys == gpstk::Position::CoordinateSystem::Cartesian) {
				strm << startPositionTag << " " << fixed << std::setprecision(5) << Coordinate.getX() << " " << fixed << std::setprecision(5) << Coordinate.getY() << " " << fixed << std::setprecision(5) << Coordinate.getZ() << endl;
				strm.lineNumber++;
			}
			else if (coorSys == gpstk::Position::CoordinateSystem::Geodetic) {
				strm << startPositionTag << " " << fixed << std::setprecision(12) << Coordinate.getGeodeticLatitude() << " " << fixed << std::setprecision(12) << Coordinate.getLongitude() << " " << fixed << std::setprecision(12) << Coordinate.getHeight() << endl;
				strm.lineNumber++;
			}

			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeStartVelocityFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 15) {
			strm << startVelocityTag << " " << fixed << std::setprecision(5) << startVelocity[0] << " " << fixed << std::setprecision(5) << startVelocity[1] << " " << fixed << std::setprecision(5) << startVelocity[2] << endl;
			strm.lineNumber++;

			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeStartAttitudeFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 16) {
			strm << startAttitudeTag << " " << fixed << std::setprecision(5) << startAttitude[0] << " " << fixed << std::setprecision(5) << startAttitude[1] << " " << fixed << std::setprecision(5) << startAttitude[2] << endl;
			strm.lineNumber++;

			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeEpochIntervalFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		if (strm.lineNumber == 18) {
			strm << EpochIntervalTag << " " << epochInterval << std::endl;
			strm.lineNumber++;
			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeEndofHeaderFound(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);
		if (strm.lineNumber == 20) {

			// Write end of header tag
			strm << endOfHeaderTag << std::endl;
			strm.lineNumber++;
			return true;
		}

		return false;
	}

	bool TrajectoryHeader::writeEmptyLine(gpstk::FFStream& ffs) const {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);
	
		// Empty line
		if (strm.lineNumber == 2  || 
			strm.lineNumber == 3  || 
			strm.lineNumber == 6  || 
			strm.lineNumber == 7  ||
			strm.lineNumber == 11 ||
			strm.lineNumber == 13 ||
			strm.lineNumber == 17 ||
			strm.lineNumber == 19 ||
			strm.lineNumber > 20)

		{
			strm << endl;
			strm.lineNumber++;

			return true;
		}

		return false;		
	}


}
