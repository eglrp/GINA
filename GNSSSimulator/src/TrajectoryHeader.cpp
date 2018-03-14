#include "stdafx.h"
#include "TrajectoryHeader.hpp"
#include "TrajectoryStream.hpp"


namespace gnsssimulator {

	


	const string TrajectoryHeader::positionFormatTypeLLH = "LLH";
	const string TrajectoryHeader::positionFormatTypeECEF = "ECEF";
	const string TrajectoryHeader::startofHeader = "GNSS Trajectory File";
	const string TrajectoryHeader::endOfHeader = "END OF HEADER";
	const string TrajectoryHeader::positionTypeLLHString = "Position LLH";
	const string TrajectoryHeader::positionTypeECEFString = "Position ECEF";
	const string TrajectoryHeader::endofHeaderString = "END OF HEADER";

	void TrajectoryHeader::reallyGetRecord(gpstk::FFStream& ffs)
		throw(exception,
			gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);
	
		// if already read, just return
		if (strm.headerRead == true)
			return;

		

		while (valid && !isHeaderEnd){
			string line;
			
			if (firstLineisRead == false){
				strm.formattedGetLine(line);
				gpstk::StringUtils::stripTrailing(line);
				if (line != startofHeader) {
					valid = false;
				}
				firstLineisRead = true;
				continue;
			}

			strm.formattedGetLine(line);
			gpstk::StringUtils::stripTrailing(line);

			if (line.length() == 0) continue;

			if ( !isPosFormatSet && line == positionTypeLLHString) {
				coorSys = gpstk::Position::CoordinateSystem::Geodetic;
				isPosFormatSet = true;
			}
			else if (!isPosFormatSet && line == positionTypeECEFString) {
				coorSys = gpstk::Position::CoordinateSystem::Cartesian;
				isPosFormatSet = true;
			}
			else if (line == endofHeaderString) {
				isHeaderEnd = true;
				strm.headerRead = true;
				strm.header = *this;
			}
		}
	}

	void TrajectoryHeader::reallyPutRecord(gpstk::FFStream& ffs) const 
		throw(exception, gpstk::FFStreamError, gpstk::StringUtils::StringException){
		TrajectoryStream& strm = dynamic_cast<TrajectoryStream&>(ffs);

		string line;

		line = startofHeader;
		strm << line << endl;
		strm.lineNumber++;

		if (coorSys == gpstk::Position::CoordinateSystem::Cartesian) {
			line = positionTypeECEFString;
			strm << line << endl;
			strm.lineNumber++;
		}
		else if (coorSys == gpstk::Position::CoordinateSystem::Geodetic) {
			line = positionTypeLLHString;
			strm << line << endl;
			strm.lineNumber++;
		}
		
		line = endOfHeader;
		strm << line << std::endl;
		strm.lineNumber++;

		return;
	}

	void TrajectoryHeader::dump(ostream& s) const {

	}
}
