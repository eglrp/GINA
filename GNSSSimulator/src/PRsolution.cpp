#include "stdafx.h"
#include "PRsolution.h"

gnsssimulator::PRsolution::PRsolution()
{
}

gnsssimulator::PRsolution::~PRsolution()
{
}

double gnsssimulator::PRsolution::getPRSolution_abs(gpstk::Triple& in_trajpos, gpstk::Triple& in_satpos)
{
	double return_pr;
	gpstk::Position pos_diff = in_trajpos - in_satpos;
	double xdiff = pos_diff.X();
	double ydiff = pos_diff.Y();
	double zdiff = pos_diff.Z();

	//Signal Travel Time
	calculate_signaltt(in_satpos, in_trajpos);

	return_pr = std::sqrt(std::pow(xdiff,2)+std::pow(ydiff,2)+std::pow(zdiff,2));

	return return_pr;
}

void gnsssimulator::PRsolution::createRinexFile(void)
{
	gpstk::Rinex3ObsHeader ref_head;
	gpstk::Rinex3ObsData ref_data;
	gpstk::Rinex3ObsStream ref_stream_in("..\\SimulatorTest\\TestFiles\\RINEX_obs\\mobs2530.17o");
	gpstk::Rinex3ObsStream out_stream("..\\Simulator\\TrajectoryTestFiles\\generatedRINEX.11o",std::ios::out);
	
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
	// TODO: Finish Data Frame creation and offload to out_stream
	Triple roverpos, satpos;
	std::vector<RinexDatum> datumvec;
	RinexDatum datum;

	ref_stream_in >> ref_data;

	for (auto& time_it : prsolutioncontainer) {
		ref_data.time = time_it.first;
		roverpos = time_it.second.first;
		ref_data.obs.clear();
		for (auto& sat_it : time_it.second.second) {
			satpos = sat_it.second;

			datum.data = getPRSolution_abs(roverpos, satpos);
			datum.dataBlank = false;
			datum.lli = 0;
			datum.lliBlank = true;
			datum.ssiBlank = false;
			datum.ssi = 0;			//Signal Strength Indicator ( Rinex 3.0+) We don't care in our case
			datumvec.clear();
			datumvec.push_back(datum);
			ref_data.obs[sat_it.first] = datumvec;
		}
		out_stream << ref_data;
	}

#pragma endregion
	
	//ref_head.dump(cout);
	cout << "[Success] Rinex Created." << endl;

	out_stream.close();
}

void gnsssimulator::PRsolution::prepareRinexData(PRSolutionContainer &container)
{
	prsolutioncontainer = container;
}

double gnsssimulator::PRsolution::getSignal_tt()
{
	return signal_tt;
}

void gnsssimulator::PRsolution::calculate_signaltt(Triple& rcv, Triple& sat)
{
	double nom = std::sqrt(std::pow((sat[0] - rcv[0]), 2) +
		std::pow((sat[1] - rcv[1]), 2) +
		std::pow((sat[2] - rcv[2]), 2));		//Geometric range

	signal_tt = nom / C_light;
}
