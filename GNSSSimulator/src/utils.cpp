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

void gnsssim_utils::prepareRinexObsFile(map < CommonTime, pair<vector<SatID>, vector<double>>> &obsContainer)
{
	this->obsContainer = obsContainer;
}
