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

	class TrajectoryData
	{
	public:

		TrajectoryData() {};

		/// destructor
		virtual ~TrajectoryData() {}

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

	};  

} 



