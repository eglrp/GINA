#pragma once

#include "TrajectoryBase.hpp"
#include "CommonTime.hpp"
#include "GPSWeekSecond.hpp"
#include "Position.hpp"

using namespace std;

namespace gnsssimulator {

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

		bool TrajectoryHeader::isPosFormatSet;
		bool TrajectoryHeader::isHeaderEnd;
		bool TrajectoryHeader::firstLineisRead;
		bool TrajectoryHeader::valid;
		gpstk::Position::CoordinateSystem coorSys;

	   

		static const string TrajectoryHeader::positionFormatTypeLLH;
		static const string TrajectoryHeader::positionFormatTypeECEF;
		static const string TrajectoryHeader::startofHeader;
		static const string TrajectoryHeader::endOfHeader;
		static const string TrajectoryHeader::positionTypeLLHString;
		static const string TrajectoryHeader::positionTypeECEFString;
		static const string TrajectoryHeader::endofHeaderString;	

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

	}; // End of class 'Rinex3NavHeader'

	   //@}

}  // End of namespace gpstk



