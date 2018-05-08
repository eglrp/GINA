#pragma once

#include "TrajectoryBase.hpp"
#include "CommonTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "Position.hpp"
#include "FFTextStream.hpp"

using namespace std;

namespace PINASimulator {

	/// This class models the RINEX 3 Nav header for a RINEX 3 Nav file.
	/// \sa Rinex3NavData and Rinex3NavStream classes.
	class TrajectoryHeader : public TrajectoryBase
	{
	public:

		//// Public member functions

		/// Constructor
		TrajectoryHeader(void) 
			: valid(true), firstLineisRead(false), isPosFormatSet(false), isHeaderEnd(false)
			{}

		/// Destructor
		virtual ~TrajectoryHeader() {}

		/// Rinex3NavHeader is a "header" so this function always returns true.
		virtual bool isHeader(void) const{
			return true;
		}

		/// This function dumps the contents of the header.
		virtual void dump(std::ostream& s) const;

		enum FormatSpec {
			isFormatPINA = 1,
			isFormatCSSIM = 2,
			isFormatGNSSSIM = 3
		} formatSpec;

		bool TrajectoryHeader::isPosFormatSet;
		bool TrajectoryHeader::isHeaderEnd;
		bool TrajectoryHeader::firstLineisRead;
		bool TrajectoryHeader::valid;
		gpstk::Position::CoordinateSystem coorSys;
		gpstk::TimeSystem timeSys;
		gpstk::Position Coordinate;
		gpstk::CommonTime startTime;
		gpstk::CommonTime endTime;

		double startVelocity[3];
		double startAttitude[3];
		double epochInterval;

		string Creator;
		string timeOfCreation;

		//TrajectoryStream strm;

		static const int TrajectoryHeader::numberofLineinHeader;
		static const string TrajectoryHeader::startofHeaderPinaTag;
		static const string TrajectoryHeader::secondLineOfPINATrajectoryTag;
		static const string TrajectoryHeader::creatorOfFileTag;
		static const string TrajectoryHeader::timeofCreationTag;
		static const string TrajectoryHeader::positionTypeLLHTag;
		static const string TrajectoryHeader::positionTypeECEFTag;
		static const string TrajectoryHeader::timeSystemDefinitionTag;
		static const string TrajectoryHeader::startTimeTag;
		static const string TrajectoryHeader::endTimeTag;
		static const string TrajectoryHeader::startPositionTag;
		static const string TrajectoryHeader::startVelocityTag;
		static const string TrajectoryHeader::startAttitudeTag;
		static const string TrajectoryHeader::EpochIntervalTag;
		static const string TrajectoryHeader::endOfHeaderTag;


	protected:

		//// Protected member functions
		/// Write this header to stream \a s.
		void reallyPutRecord(gpstk::FFStream& s) const
			throw(exception,
				gpstk::FFStreamError,
				gpstk::StringUtils::StringException);


		/// This function reads the RINEX Nav header from the given FFStream.
		/// If an error is encountered in reading from the stream, the stream
		/// is reset to its original position and its fail-bit is set.
		/// @throws StringException when a StringUtils function fails
		/// @throws FFStreamError when exceptions(failbit) is set and a read
		///         or formatting error occurs.  This also resets the stream
		///         to its pre-read position.
		void reallyGetRecord(gpstk::FFStream& s)
			throw(exception,
				gpstk::FFStreamError,
				gpstk::StringUtils::StringException);

	private:

		// Read methods
		bool hasStartofHeaderFound(string&, gpstk::FFStream&);
		bool hasCreatorFound(string&);
		bool hasTimeofCreationFound(string&);
		bool hasCoordinateSystemFound(string&);
		bool hasTimeSystemFound(string&);
		bool hasStartTimeFound(string&);
		bool hasEndTimeFound(string&);
		bool hasStartPositionFound(string&);
		bool hasStartVelocityFound(string&);
		bool hasStartAttitudeFound(string&);
		bool hasEpochIntervalFound(string&);
		bool hasEndofHeaderFound(string&, gpstk::FFStream&);

		// Write methods
		bool writeStartofHeader(gpstk::FFStream&) const;
		bool writeCreatorFound(gpstk::FFStream&) const;
		bool writeTimeofCreationFound(gpstk::FFStream&) const;
		bool writeTimeSystemFound(gpstk::FFStream&) const;
		bool writeStartTimeFound(gpstk::FFStream&) const;
		bool writeEndTimeFound(gpstk::FFStream&) const;
		bool writeCoordinateSystemFound(gpstk::FFStream&) const;
		bool writeStartPositionFound(gpstk::FFStream&) const;
		bool writeStartVelocityFound(gpstk::FFStream&) const;
		bool writeStartAttitudeFound(gpstk::FFStream&) const;
		bool writeEpochIntervalFound(gpstk::FFStream&) const;
		bool writeEndofHeaderFound(gpstk::FFStream&) const;
		bool writeEmptyLine(gpstk::FFStream&) const;

		

	}; // End of class 'Rinex3NavHeader'

	   //@}

}  // End of namespace gpstk



