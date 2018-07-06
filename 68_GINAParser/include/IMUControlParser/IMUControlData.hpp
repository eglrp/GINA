#pragma once


#include <list>

#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "RinexNavBase.hpp"
#include "IMUControlBase.hpp"
#include "IMUControlStream.hpp"
#include "Position.hpp"
#include "IMUControlHeader.hpp"

#include "I_GINA_writer.hpp"

namespace GINASimulator
{
	/// @ingroup FileHandling
	//@{

	/**
	* This class models a RINEX NAV record.
	*
	* \sa rinex_nav_test.cpp and rinex_nav_read_write.cpp for examples.
	* \sa gpstk::RinexNavHeader and gpstk::RinexNavStream classes.
	*/
	class IMUControlData : public IMUControlBase
	{
	public:
		/**
		* Constructor
		* @warning CHECK THE PRNID TO SEE IF THIS DATA IS
		*  VALID BEFORE USING!!
		*/
		IMUControlData() {};

		/// destructor
		virtual ~IMUControlData() {}

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
	
		bool compare(const IMUControlData&) const;
		bool operator==(const IMUControlData& ) const;
		bool operator!=(const IMUControlData& ) const;
		
		IMUControlData& operator=(IMUControlData&);

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;
		gpstk::TimeSystem timeSys = gpstk::TimeSystem::Systems::Unknown;

		gpstk::CommonTime startTime;
		gpstk::CommonTime endTime;
		double timeStep;

		unsigned int precision = 11;
		double acceleration[3];
		double angularRate[3];
		
		double getStartGPSWeek(void);
		double getStartGPSToW(void);

		double getEndGPSWeek(void);
		double getEndGPSToW(void);

		double getTimeStep(void);

		static const string IMUControlData::startofDataTag;
		IMUControlData& operator=(I_IMUControlData&);
		
	private:
		IMUControlStream* strm;

		void IMUControlData::parseLine(std::string& currentLine)
			throw(gpstk::StringUtils::StringException, gpstk::FFStreamError);

	};  // class RinexNavData

		//@}
} // namespace



