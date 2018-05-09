#pragma once

#include "trajectoryReader.h"
#include "CoordinateFrameHandler.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Rinex3ObsData.hpp"
#include "Rinex3NavData.hpp"
#include "RinexSatID.hpp"
#include "GPSEphemerisStore.hpp"
#include "Xvt.hpp"
#include "GPSWeek.hpp"
#include "GPSWeekZcount.hpp"
#include "GPSWeekSecond.hpp"

using namespace gpstk;

// v2 map
typedef std::map<SatID, std::map<CivilTime, double>> PseudoRangeContainer;


class satDataContainer {

public:
	satDataContainer();
	~satDataContainer();

	/* When reading in RINEX line by line, pass the pseudorange for each epoch 
		for each Satellite
		@Effect: Container[Satid][Civiltime] = double
	*/
	void assemblePseudoRangeContainer(SatID, CivilTime, double);
	
	/* Pass EphemerisStore reference to SatDataContainer.
		EphStore stores all the ephemeris data for each satellite.
	*/
	void passEphemerisStore(GPSEphemerisStore&);

	/* Get the full epoch list for a given satellite
		@Param The SatID object
		@return The <vector> containing all the epoch times for the given SatID
	*/
	std::vector<CivilTime> getEpochVectorforSat(SatID&);

	/* Get the list of stored SVs as a vector*/
	std::vector<SatID> getSatIDvectorlist(void);

	/* Get the ephemerisStore fro satDataContainer*/
	GPSEphemerisStore& getEphemerisStore();

	/* Check if a given epoch is outside of the associated epoch observation vector
	* Returns TRUE if satellite is not observable in the given epoch.
	* @param : CivilianTime in question
	* @param : Time vectors of the given sat.
	*/
	bool isEpochonDarkSide(CivilTime civiliantime, std::vector<CivilTime>& referenceEpochs);
	
	/* Get Satellite ID object
	* Satellites are ordered and stored inside the DataContainer object.
	* This function iterates through it and returns the ith satellite stored.
	*/
	SatID getSatIDObject(int, SatID::SatelliteSystem );

	/* Create a CivilianTime object from input time as arguments
		@return:CivilTime
	*/
	CivilTime getCivilTimeObject(int year, int month, int day, int hour, int minute, double second);
	
	/* Query Ephemeris Data for a [SatID, CivilianTime] combination
		Time can be any valid CivilianTime object. If Navigation Data
		from RINEX file for the queried epoch is not available,
		GPSTK uses built-in models to calculate Satellite position.
		@return: Orbiteph
	*/
	OrbitEph getSatInfoAtEpoch(SatID&, CivilTime&);

	/* Get the C1 PseudoRange stored. Can return only values
		that were stored in the RINEX file.
		@ Return double [meters]
	*/
	double getPseudorangeatEpoch(SatID, CivilTime&);

	PseudoRangeContainer getPseudorangeContainer();

private:
	
	GPSEphemerisStore ephemerisStore;
	GPSEphemeris ephemeris;
	RinexSatID sat;
	SatID templatesat;
	//vector<SatID> satIDContainer;

	/* Contains the C1 Pseudorange for every RINEX epoch for each satellite
	*/
	PseudoRangeContainer pseudoRangeContainer;

	/*
	*/
	void satID_check_isPresent();


	
	

};

