#pragma once

#include "IMUControlBase.hpp"
#include "CommonTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "Position.hpp"
#include "FFTextStream.hpp"

using namespace std;

namespace GINAParser {

	/// This class models the RINEX 3 Nav header for a RINEX 3 Nav file.
	/// \sa Rinex3NavData and Rinex3NavStream classes.
	class IMUControlHeader : public IMUControlBase
	{
	public:

		//// Public member functions

		/// Constructor
		IMUControlHeader(void)
			: valid(true), firstLineisRead(false), isPosFormatSet(false), isHeaderEnd(false)
			{}

		/// Destructor
		virtual ~IMUControlHeader() {}

		/// Rinex3NavHeader is a "header" so this function always returns true.
		virtual bool isHeader(void) const{
			return true;
		}

		/// This function dumps the contents of the header.
		//virtual void dump(std::ostream& s) const;

	
		bool IMUControlHeader::isPosFormatSet;
		bool IMUControlHeader::isHeaderEnd;
		bool IMUControlHeader::firstLineisRead;
		bool IMUControlHeader::valid;
		gpstk::Position::CoordinateSystem coorSys;
		gpstk::TimeSystem timeSys;
		gpstk::Position startPosition;
		gpstk::CommonTime startTime;
		gpstk::CommonTime endTime;

		double startVelocity[3];
		double startAttitude[3];

		string Creator;
		string timeOfCreation;

		//TrajectoryStream strm;

		static const int IMUControlHeader::numberofLineinHeader;
		static const string IMUControlHeader::startofHeaderGinaTag;
		static const string IMUControlHeader::secondLineOfIMUTrajectoryTag;
		static const string IMUControlHeader::creatorOfFileTag;
		static const string IMUControlHeader::timeofCreationTag;
		static const string IMUControlHeader::positionTypeLLHTag;
		static const string IMUControlHeader::positionTypeUnknownTag;
		static const string IMUControlHeader::positionTypeECEFTag;
		static const string IMUControlHeader::timeSystemDefinitionTag;
		static const string IMUControlHeader::startTimeTag;
		static const string IMUControlHeader::endTimeTag;
		static const string IMUControlHeader::startPositionTag;
		static const string IMUControlHeader::startVelocityTag;
		static const string IMUControlHeader::startAttitudeTag;
		static const string IMUControlHeader::endOfHeaderTag;


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
		bool writeEndofHeaderFound(gpstk::FFStream&) const;
		bool writeEmptyLine(gpstk::FFStream&) const;

		

	}; // End of class 'Rinex3NavHeader'

	   //@}

}  // End of namespace gpstk



