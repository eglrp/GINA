#pragma once

#include "GINAConfig.h"

#include <list>

#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "RinexNavBase.hpp"
#include "Position.hpp"
#include GINA_PARSER_TrajectoryData_hpp

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
	class TrajectoryData
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
	
		bool compare(const TrajectoryData&) const;
		bool operator==(const TrajectoryData& ) const;
		bool operator!=(const TrajectoryData& ) const;
		
		TrajectoryData& operator+=(gpstk::Position&);
		TrajectoryData& TrajectoryData::operator=(gpstk::Position&);
		TrajectoryData& TrajectoryData::operator=(GINAParser::TrajectoryData&);

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;
		gpstk::Position pos;
		gpstk::GPSWeekSecond gpsTime;
	
		
	private:

		

	};  // class RinexNavData

		//@}
} // namespace



