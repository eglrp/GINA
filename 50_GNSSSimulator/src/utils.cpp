#include "utils.h"

gnsssim_utils::gnsssim_utils()
{

}

gnsssim_utils::~gnsssim_utils()
{
}

void gnsssim_utils::createRinexObsFile()
{
	std::cout << "Rinex Header creation started..." << std::endl;

	gpstk::Rinex3ObsHeader ref_head;
	gpstk::Rinex3ObsData ref_data;
	gpstk::Rinex3ObsStream ref_stream_in("..\\..\\GNSSSimulator\\TrajectoryTestFiles\\input_referenceobs_mobs2530.17o");
	gpstk::Rinex3ObsStream out_stream("..\\..\\GNSSSimulator\\TrajectoryTestFiles\\output_createdRinexObs.17o", std::ios::out);

	ref_stream_in >> ref_head;

#pragma region Header Manipulation
	ref_head.fileProgram = "GNSS Simulator PR";
	ref_head.agency = "---";

	ref_head.commentList.clear();
	ref_head.commentList.push_back("C1 modelled using GNSSSIM");
	ref_head.commentList.push_back("https://github.com/GNSSSim/GNSSSimulator");
	ref_head.commentList.push_back("Header contents might be invalid. Only C1 is important.");
	ref_head.obsTypeList.clear();

	RinexObsID obsID;
	gpstk::Rinex3ObsHeader::RinexObsVec obsvec;

	obsID.type = ObsID::otRange;
	obsID.band = ObsID::cbL1;
	obsID.code = ObsID::tcCA;
	obsvec.push_back(obsID);
	ref_head.mapObsTypes.clear();
	ref_head.mapObsTypes["G"] = obsvec;

	ref_head.R2ObsTypes.clear();
	ref_head.R2ObsTypes.push_back("C1");
#pragma endregion

	out_stream << ref_head;

#pragma region Data Manipulation
	Triple roverpos, satpos;
	std::vector<RinexDatum> datumvec;
	RinexDatum datum;

	ref_stream_in >> ref_data;

	for (auto& time_it : obsContainer) {
		ref_data.time = time_it.first;
		//roverpos = time_it.second.first;
		ref_data.obs.clear();
		//for (auto& sat_it : time_it.second.first) {
		//for (std::vector<SatID>::iterator sat_it = time_it.second.first.begin(); sat_it <= time_it.second.first.end(); sat_it++) {
		for(int sat_it = 0; sat_it < time_it.second.first.size();sat_it++){
			////satpos = sat_it.second;
			//double prange = time_it.second.second.at(sat_it);
			datum.data = time_it.second.second.at(sat_it); //Pseudorange
			datum.dataBlank = false;
			datum.lli = 0;
			datum.lliBlank = true;
			datum.ssiBlank = false;
			datum.ssi = 0;			//Signal Strength Indicator ( Rinex 3.0+) We don't care in our case
			datumvec.clear();
			datumvec.push_back(datum);
			ref_data.obs[time_it.second.first.at(sat_it)] = datumvec;
		}
		out_stream << ref_data;
	}

#pragma endregion

	std::cout << "[Success] Rinex Created." << std::endl;

	out_stream.close();

	std::cout << "Rinex Header creation finished..." << std::endl;
}


void gnsssim_utils::createRinexObsFile(const char* fileNamewPath) {
	
	gpstk::Rinex3ObsStream outStream(fileNamewPath, std::ios::out);

	SystemTime sysTime;
	gpstk::Rinex3ObsHeader ref_head;
	gpstk::Rinex3ObsData ref_data;

	ref_head.valid = 0;
	ref_head.version = 3.01;
	
	ref_head.fileProgram = "GINA GNSS Simulator";
	ref_head.fileAgency = "Robert Bosch Gmbh.";
	
	string currDate = printTime(sysTime, "%04Y%02m%02d %02H%02M%02S %P");
	ref_head.date = currDate;
	ref_head.preserveDate = true;

	ref_head.commentList.clear();
	ref_head.commentList.push_back("C1 modelled using GNSS SIM");
	ref_head.commentList.push_back("https://github.com/GINAProject/GINA");
	ref_head.commentList.push_back("Header contents might be invalid in some cases.");

	ref_head.markerName = "---";
	ref_head.markerType = "---";

	ref_head.observer = "CC/ENG2";
	ref_head.agency = "VMPS System Simulation";

	ref_head.recNo = "---";
	ref_head.recType = "---";
	ref_head.recVers = "1.0.0";

	ref_head.antNo = "---";
	ref_head.antType = "No modelled antenna";

	ref_head.antennaPosition = Triple(0.0, 0.0, 0.0);
	ref_head.antennaDeltaHEN = Triple(0.0, 0.0, 0.0);

	ref_head.obsTypeList.clear();
	RinexObsID obsID;
	gpstk::Rinex3ObsHeader::RinexObsVec obsvec;

	obsID.type = ObsID::otRange;
	obsID.band = ObsID::cbL1;
	obsID.code = ObsID::tcCA;
	obsvec.push_back(obsID);
	ref_head.mapObsTypes.clear();
	ref_head.mapObsTypes["G"] = obsvec;

	ref_head.R2ObsTypes.clear();
	ref_head.R2ObsTypes.push_back("C1");

	ref_head.firstObs = this->obsContainer.begin()->first;
	ref_head.lastObs = prev(this->obsContainer.end(),1)->first;

	ref_head.numSVs = this->numSVinObs;

	ref_head.sigStrengthUnit = "1";
	ref_head.interval = next(this->obsContainer.begin(),1)->first - this->obsContainer.begin()->first;
	//ref_head.sysPhaseShift = 
	ref_head.validEoH = true;
	
	ref_head.valid |= ref_head.validVersion;
	ref_head.valid |= ref_head.validRunBy;
	ref_head.valid |= ref_head.validComment;
	ref_head.valid |= ref_head.validMarkerName;
	ref_head.valid |= ref_head.validMarkerNumber;
	ref_head.valid |= ref_head.validMarkerType;
	ref_head.valid |= ref_head.validObserver;
	ref_head.valid |= ref_head.validReceiver;
	ref_head.valid |= ref_head.validAntennaPosition;
	ref_head.valid |= ref_head.validAntennaDeltaHEN;
	ref_head.valid |= ref_head.validNumObs;
	ref_head.valid |= ref_head.validSystemNumObs;
	ref_head.valid |= ref_head.validSigStrengthUnit;
	ref_head.valid |= ref_head.validInterval;
	ref_head.valid |= ref_head.validFirstTime;
	ref_head.valid |= ref_head.validLastTime;
	ref_head.valid |= ref_head.validNumSats;
	ref_head.valid |= ref_head.validPrnObs;
	//ref_head.valid |= ref_head.allValid2;
	//ref_head.valid |= ref_head.allValid30;
	//ref_head.valid |= ref_head.allValid301;
	ref_head.valid |= ref_head.allValid302;
	

	//ref_head.putRecord(outStream);
	ref_head.writeHeaderRecords(outStream);

	//outStream << ref_head;


#pragma region Data Manipulation
	Triple roverpos, satpos;
	std::vector<RinexDatum> datumvec;
	RinexDatum datum;
	//RinexObsID obsID(RinexObsHeader::C1);

	for (auto& time_it : obsContainer) {
		ref_data.time = time_it.first;
		//roverpos = time_it.second.first;
		ref_data.obs.clear();
		//for (auto& sat_it : time_it.second.first) {
		//for (std::vector<SatID>::iterator sat_it = time_it.second.first.begin(); sat_it <= time_it.second.first.end(); sat_it++) {
		for (int sat_it = 0; sat_it < time_it.second.first.size();sat_it++) {
			////satpos = sat_it.second;
			//double prange = time_it.second.second.at(sat_it);
			datum.data = time_it.second.second.at(sat_it); //Pseudorange
			datum.dataBlank = false;
			datum.lli = 0;
			datum.lliBlank = true;
			datum.ssiBlank = false;
			datum.ssi = 0;			//Signal Strength Indicator ( Rinex 3.0+) We don't care in our case
			
			datumvec.push_back(datum);
			//ref_data.obs[time_it.second.first.at(sat_it)] = datumvec;

			ref_data.setObs(datum, time_it.second.first.at(sat_it),RinexObsID(RinexObsHeader::C1),ref_head );
		}
		ref_data.auxHeader = ref_head;
		ref_data.clockOffset = 0.0;
		ref_data.epochFlag = 0;
		ref_data.numSVs = time_it.second.first.size();
		//ref_data.putRecord(outStream);
		datumvec.clear();
		outStream << ref_data;
		
	}
	outStream.close();
#pragma endregion
}

void gnsssim_utils::prepareRinexObsFile(map < CommonTime, pair<vector<SatID>, vector<double>>> &obsContainer)
{
	this->obsContainer = obsContainer;
	countSatellites(this->obsContainer);
}

int gnsssim_utils::countSatellites(ObsContainer & container)
{
	vector<int> fullSatVec;

	for (auto &time_it : container)
	{
		for (auto &sat_it : time_it.second.first)
		{
			fullSatVec.push_back(sat_it.id);		
		}
	}
	sort(fullSatVec.begin(), fullSatVec.end());
	fullSatVec.erase(unique(fullSatVec.begin(), fullSatVec.end()), fullSatVec.end());

	this->numSVinObs = fullSatVec.size();

	return fullSatVec.size();
}
