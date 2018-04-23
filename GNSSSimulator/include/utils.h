#pragma once

#include "stdafx.h"
#include <iostream>
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavData.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"
#include "Rinex3ObsStream.hpp"


//using namespace gnsssimulator;
using namespace gpstk;
using namespace std;


class gnsssim_utils {

public:
	gnsssim_utils();
	~gnsssim_utils();

	void createRinexObsFile();
	void prepareRinexObsFile(map < CommonTime, pair<vector<SatID>, vector<double>>>& obsContainer);

	map < CommonTime, pair<vector<SatID>, vector<double>>> obsContainer;
private:
	map<CommonTime, vector<double>> obsData;
	bool isHeaderWritten = false;

};