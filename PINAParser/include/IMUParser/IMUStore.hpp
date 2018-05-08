#pragma once

#include <iostream>
#include <map>
#include "IMUData.hpp"

namespace PINASimulator {
	class IMUStore
	{
	public:
		IMUStore();
		~IMUStore();

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;

		IMUData& addPosition(IMUData);
		IMUData findPosition(gpstk::GPSWeekSecond);
		/* Return epochs as a vector from the Trajectoryfile.
		*/
		vector<gpstk::GPSWeekSecond> listTime(void); // TODO tudja kilistázni a benne levo idoket.

		bool IMUStore::operator==(const IMUStore& other) const;
		bool IMUStore::operator!=(const IMUStore& other) const;

	protected:
		bool isCoorSystemSet = false;
		typedef std::map<gpstk::GPSWeekSecond, IMUData> IMUMap;
		IMUMap imuStore;
		void setCorrdinateSystem(gpstk::Position::CoordinateSystem);
		bool compare(const IMUStore& other) const;
	};

}