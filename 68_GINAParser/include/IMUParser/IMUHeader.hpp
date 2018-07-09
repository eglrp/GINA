#pragma once

#include "IMUBase.hpp"
#include "CommonTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "Position.hpp"
#include "FFTextStream.hpp"

using namespace std;

#define INVALID_EPOCHINTERVAL_IMU_HEADER -1

namespace GINAParser {

	/// This class models the RINEX 3 Nav header for a RINEX 3 Nav file.
	/// \sa Rinex3NavData and Rinex3NavStream classes.
	class IMUHeader : public IMUBase
	{
	public:

		//// Public member functions

		/// Constructor
		IMUHeader(void)
			: valid(true), firstLineisRead(false), isPosFormatSet(false), isHeaderEnd(false)
			{}

		/// Destructor
		virtual ~IMUHeader() {}

		/// Rinex3NavHeader is a "header" so this function always returns true.
		virtual bool isHeader(void) const{
			return true;
		}

		/// This function dumps the contents of the header.
		//virtual void dump(std::ostream& s) const;

	
		bool IMUHeader::isPosFormatSet;
		bool IMUHeader::isHeaderEnd;
		bool IMUHeader::firstLineisRead;
		bool IMUHeader::valid;
		gpstk::Position::CoordinateSystem coorSys;
		gpstk::TimeSystem timeSys;
		gpstk::Position startPosition;
		gpstk::CommonTime startTime;
		gpstk::CommonTime endTime;

		double startVelocity[3];
		double startAttitude[3];
		double epochInterval;

		string Creator;
		string timeOfCreation;

		//TrajectoryStream strm;

		static const int IMUHeader::numberofLineinHeader;
		static const string IMUHeader::startofHeaderGinaTag;
		static const string IMUHeader::secondLineOfIMUTrajectoryTag;
		static const string IMUHeader::creatorOfFileTag;
		static const string IMUHeader::timeofCreationTag;
		static const string IMUHeader::positionTypeLLHTag;
		static const string IMUHeader::positionTypeUnknownTag;
		static const string IMUHeader::positionTypeECEFTag;
		static const string IMUHeader::timeSystemDefinitionTag;
		static const string IMUHeader::startTimeTag;
		static const string IMUHeader::endTimeTag;
		static const string IMUHeader::startPositionTag;
		static const string IMUHeader::startVelocityTag;
		static const string IMUHeader::startAttitudeTag;
		static const string IMUHeader::EpochIntervalTag;
		static const string IMUHeader::endOfHeaderTag;


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


		void dump(ostream& s) const;

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



