#pragma once

#include "Rinex3ObsBase.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"
#include "Rinex3ObsStream.hpp"
#include "RinexDatum.hpp"
#include "CivilTime.hpp"
#include "GNSSconstants.hpp"


class RinexReader {

public:
	RinexReader();
	~RinexReader();

	void readFile(char*);
private:
	char* inputpath;
	char* outputpath;
	


};