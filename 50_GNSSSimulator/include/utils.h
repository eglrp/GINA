#pragma once

#include "stdafx.h"
#include <iostream>
#include "SystemTime.hpp"
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavData.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"
#include "Rinex3ObsStream.hpp"
#include "TimeString.hpp"


//using namespace gnsssimulator;
using namespace gpstk;
using namespace std;



class gnsssim_utils {

public:
	gnsssim_utils();
	~gnsssim_utils();

	void createRinexObsFile();
	/* Create Observation File from scratch
	*/
	void createRinexObsFile1();
	/* Pass Container structure to Utils
	*/
	void prepareRinexObsFile(map < CommonTime, pair<vector<SatID>, vector<double>>>& obsContainer);

	typedef map < CommonTime, pair<vector<SatID>, vector<double>>> ObsContainer;
	ObsContainer obsContainer;
private:
	map<CommonTime, vector<double>> obsData;
	int numSVinObs;
	bool isHeaderWritten = false;

	int countSatellites(ObsContainer &container);
	//TODO: Apply value with prepareRienxObsFile()
	//Rinex3ObsStream outStream;

	
};