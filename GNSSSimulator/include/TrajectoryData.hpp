#pragma once


#include <list>

#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "RinexNavBase.hpp"
#include "TrajectoryBase.hpp"
#include "TrajectoryStream.hpp"
#include "Position.hpp"

namespace gnsssimulator
{
	/// @ingroup FileHandling
	//@{

	/**
	* This class models a RINEX NAV record.
	*
	* \sa rinex_nav_test.cpp and rinex_nav_read_write.cpp for examples.
	* \sa gpstk::RinexNavHeader and gpstk::RinexNavStream classes.
	*/
	class TrajectoryData : public TrajectoryBase
	{
	public:
		/**
		* Constructor
		* @warning CHECK THE PRNID TO SEE IF THIS DATA IS
		*  VALID BEFORE USING!!
		*/
		TrajectoryData() {};

		/// destructor
		virtual ~TrajectoryData() {}

		// The next four lines is our common interface
		/// RinexNavData is "data" so this function always returns true.
		virtual bool isData() const { return true; }

		/// Outputs the record to the FFStream \a s.
		virtual void reallyPutRecord(gpstk::FFStream& s) const
			throw(std::exception, gpstk::FFStreamError,
				gpstk::StringUtils::StringException);

		/**
		* This function retrieves a RINEX NAV record from the given FFStream.
		* If an error is encountered in reading from the stream, the stream
		* is returned to its original position and its fail-bit is set.
		* @throws StringException when a StringUtils function fails
		* @throws FFStreamError when exceptions(failbit) is set and
		*  a read or formatting error occurs.  This also resets the
		*  stream to its pre-read position.
		*/
		virtual void reallyGetRecord(gpstk::FFStream& s)
			throw(std::exception, gpstk::FFStreamError,
				gpstk::StringUtils::StringException);
	
		bool compare(const TrajectoryData&) const;
		bool operator==(const TrajectoryData& ) const;
		bool operator!=(const TrajectoryData& ) const;
		

		TrajectoryData& operator+=(gpstk::Position&);

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;
		gpstk::Position pos;
		gpstk::GPSWeekSecond gpsTime;
	
		
	private:
		void TrajectoryData::getPosition(const std::string& currentLine)
			throw(gpstk::StringUtils::StringException, gpstk::FFStreamError);

		void TrajectoryData::getTime(const std::string& currentLine)
			throw(gpstk::StringUtils::StringException, gpstk::FFStreamError);

		

	};  // class RinexNavData

		//@}
} // namespace



