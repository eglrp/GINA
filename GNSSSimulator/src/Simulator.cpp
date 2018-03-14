// Simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Simulator.h"
#include "Navigation_examples.h"
#include "Test_Trajectory_Class.hpp"
#include "Test_simulatePseudoRange.hpp"

const int ProcessRinex = 1;
const int ProcessTrajectory = 1;
const int Solution_to_RINEX = 0;
const int Run_Tests = 1;

using namespace gpstk;
using namespace std;


satDataContainer satDataContainer_c;		//Stores C1 and Ephemeris Data for the SVs
GPSEphemerisStore bceStore;
gnsssimulator::TrajectoryStore trajStore;
gnsssimulator::PRsolution prsolution;

//typedef map<CivilTime, pair<Triple, map<SatID, Triple>>> PRSolutionContainer;
typedef map<SatID, Triple> SatDataEpoch;
typedef pair<Triple, SatDataEpoch> SolutionDataBlock;

int main(int argc, char **argv)
{
	std::cout << "CONFIG: " << "ProcessRinex " << ProcessRinex << " ProcessTrajectory: " << ProcessTrajectory <<
		" Solution to RINEX: " << Solution_to_RINEX << " Run Tests: " << Run_Tests << endl;

	/// Function Declarations
	int ProcessFiles();
	int ProcessTrajectoryFile();
	bool Test_ReProcess_Solution_Rinex();
	/// End of Declarations
	if (Run_Tests) {
		//Navigation_examples_1();
		
		//Test_Trajectory_1();
		//Test_Trajectory_2();
		//Test_Trajectory_3();
		//Test_Trajectory_4();
		//Test_Trajectory_5();
		//Test_Trajectory_6();
		//Test_Trajectory_7();
		//Test_Trajectory_8();
		//Test_Trajectory_9();
		//Test_Trajectory_10();
		//Test_Trajectory_11();
		//Test_Trajectory_12();
		//Test_Trajectory_13();
		//Test_Trajectory_14();
		//Test_Trajectory_15();

		//makeSimplePseudoRange();
	}
	
	
	/// Read in RINEX files
	if (ProcessRinex)
		ProcessFiles();
	/// Read Rover Trajectory file
	if (ProcessTrajectory)
		ProcessTrajectoryFile();

			//DEBUG FOR RINEX PROCESS
	cout << endl << endl << "------------" << endl;
	OrbitEph query_ephemeris;
	CivilTime query_time = satDataContainer_c.getCivilTimeObject(2017, 9, 10, 1, 13, 37.456789);
	SatID query_sat = satDataContainer_c.getSatIDObject(4, SatID::systemGPS);
	try
	{
		query_ephemeris = satDataContainer_c.getSatInfoAtEpoch(query_sat, query_time);
		cout << "EPHQuery: " << query_ephemeris.svXvt(query_time) << endl;
		Xvt xvt_data = satDataContainer_c.getEphemerisStore().getXvt(query_sat, query_time);
		cout << "StoreQuery: " << xvt_data.x << endl;
	}
	catch (const std::exception& e)
	{
		cout << endl << e.what();
	}
	
	// Calculate base pseudoranges and assign them to the container
#pragma region Pseudorange 0th Solution
	vector<GPSWeekSecond>traj_timevec = trajStore.listTime();
	double Prange;

	SatDataEpoch satDataEpoch;												// Sat Data map<SatID,SatPosition>
	SolutionDataBlock solutionDataBlock;									//+RoverPos : Pair<Triple,SatDataEpoch>
	gnsssimulator::PRsolution::PRSolutionContainer prsolutionContainer;		// The whole solution container

	double Error_overcorr = 0;

	for (auto& it : traj_timevec) {
		CivilTime civtime = it.convertToCommonTime();
		std::cout << setw(30) << endl;
		std::cout << "Next Epoch: " << endl;
		std::cout << it << "  converted to civtime: " << civtime << endl;

		gnsssimulator::TrajectoryData data = trajStore.findPosition(it);
		std::cout << "Rover Position:     " << data.pos << endl << endl;

		for (auto& satid_it : satDataContainer_c.getSatIDvectorlist()) { // TODO 
			try
			{
				Xvt xvt_data;
				try
				{
					civtime = it.convertToCommonTime();	//Reset civtime
					xvt_data = satDataContainer_c.getEphemerisStore().getXvt(satid_it, civtime); // TODO satDataContainer_c nem szukseges
					
					xvt_data = satDataContainer_c.getEphemerisStore().getXvt(satid_it, civtime);// TODO satDataContainer_c nem szukseges
					Prange = prsolution.getPRSolution_abs(data.pos, xvt_data.x);
					
					CivilTime civtime_temp = civtime;
					// iterative
					double t_trans = Prange / 300000000;
					civtime_temp.second = civtime.second - t_trans; // TODO minutes rollover
					xvt_data = satDataContainer_c.getEphemerisStore().getXvt(satid_it, civtime_temp);
					Prange = prsolution.getPRSolution_abs(data.pos, xvt_data.x);

					t_trans = Prange / 300000000;
					civtime_temp.second = civtime.second - t_trans;
					xvt_data = satDataContainer_c.getEphemerisStore().getXvt(satid_it, civtime_temp);
					Prange = prsolution.getPRSolution_abs(data.pos, xvt_data.x);

					t_trans = Prange / 300000000;
					civtime_temp.second = civtime.second - t_trans;
					xvt_data = satDataContainer_c.getEphemerisStore().getXvt(satid_it, civtime_temp);
					Prange = prsolution.getPRSolution_abs(data.pos, xvt_data.x);
					//

					Error_overcorr = xvt_data.clkbias + xvt_data.relcorr;
					civtime.second += Error_overcorr; // Add error because PRSol2 automatically calculates with these;
					// TODO eredeti civtime kell. meg korrekcio elott
					satDataEpoch[satid_it] = xvt_data.x;
				}
				catch (...)
				{
					// TODO: Put a default value for satDataEpoch
					//Triple def(1.0e7, 1.0e7, 1.0e7);
					Triple def(NULL, NULL, NULL);
					Prange = 0;
					satDataEpoch[satid_it] = def;
				}
				

				std::cout << " Sat ID: " << satid_it << " Position: "
					<< satDataEpoch.at(satid_it) << " PseudoRange: " << Prange
					<< " Signal tt: " << prsolution.getSignal_tt()
					<< endl;
			}
			catch (const gpstk::InvalidRequest& e)
			{				
				std::cout << "[Warning] Can't get OrbitEph for " << satid_it << " at: " << civtime << endl;
			}
		}
		solutionDataBlock.first = data.pos;
		solutionDataBlock.second = satDataEpoch;
		prsolutionContainer[civtime] = solutionDataBlock;
	}
#pragma endregion

	//Calculate Site position using built-in RAIM
#pragma region PseudoRange RaimCompute Solution
	ofstream ostrm("..\\Simulator\\TrajectoryTestFiles\\output_RaimSolution.txt", std::ios::out);	//Output file
	PRSolution2 RaimSolver;
	//PRSolution RaimSolver1;
	vector<double> prvector;
	ZeroTropModel zeroTrop;
	TropModel *tropModelPtr = &zeroTrop;
	CivilTime correctedCivtime; // TODO nevezek fura, felrevezeto
	for (auto& it : traj_timevec) {
		CivilTime civtime = it.convertToCommonTime();
		prvector.clear();
		Xvt xvt_data;	//For error correction
		double errorcorr;
		for (auto& satid_it : satDataContainer_c.getSatIDvectorlist()) { // TODO mint a fenntieknel

				double pr_obs;
				double pr_calc;

				// Correct the time with ClockBias and Relativity
				
				try
				{
					xvt_data = satDataContainer_c.getEphemerisStore().getXvt(satid_it, civtime);
					errorcorr = xvt_data.getClockBias() + xvt_data.getRelativityCorr();
				}
				catch (gpstk::InvalidRequest& e)
				{
					errorcorr = 0.0;
				}
				// corrected time was here 10.24
				

				//prvector.push_back(satDataContainer_c.getPseudorangeatEpoch(satid_it, civtime));
				Triple roverpos = prsolutionContainer[civtime].first;
				Triple satPos = prsolutionContainer[civtime].second[satid_it];
				pr_calc = prsolution.getPRSolution_abs(roverpos,satPos);	// TODO: Might cause the 40km issue

				try		// TODO: obs rinex pr is not needed
				{
					pr_obs = satDataContainer_c.getPseudorangeatEpoch(satid_it, civtime);
				}
				catch (const std::exception&)
				{
					pr_obs = 0.0;
				}

				prvector.push_back(pr_calc);
			}
		correctedCivtime = civtime;
		//correctedCivtime.second += errorcorr;
			

		std::cout << "RaimCompute started." << endl;
		std::cout << RaimSolver.RAIMCompute(civtime, satDataContainer_c.getSatIDvectorlist(), prvector, bceStore, tropModelPtr) << endl;
		std::cout << std::setprecision(12) << RaimSolver.Solution[0] << " " <<
			std::setprecision(12) << RaimSolver.Solution[1] << "  " <<
			std::setprecision(12) << RaimSolver.Solution[2] << endl;
		ostrm << std::setprecision(12) << RaimSolver.Solution[0] << " " << std::setprecision(12) << RaimSolver.Solution[1]
			<< " " << std::setprecision(12) << RaimSolver.Solution[2] << endl;
		
	}
	ostrm.close();
#pragma endregion

	if (Solution_to_RINEX) {
		std::cout << "Creating Rinex File. " << endl;
		prsolution.prepareRinexData(prsolutionContainer);
		prsolution.createRinexFile();
	}

	if (Run_Tests) {
		std::cout << "Solution Reprocess ?= Original Rinex Process: " << Test_ReProcess_Solution_Rinex() << endl;
	}
	
	return 0;
}

// TODO: Rename to ProcessRinexFile and refactor code to handle Rinex dynamically.
int ProcessFiles(void) throw(Exception)
{
	try {
		int iret;
		iret = 0;
		int indexC1;
		Rinex3ObsStream istrm;
		Rinex3ObsHeader Rhead, Rheadout;
		Rinex3ObsData Rdata;
		RinexDatum dataobj;

		Rinex3NavStream inavstrm;
		Rinex3NavHeader Rnavhead;
		Rinex3NavData Rnavdata;

		Xvt xvt_data;

		string filepath_obs("..\\SimulatorTest\\TestFiles\\RINEX_obs\\mobs2530.17o");
		string filepath_nav("..\\SimulatorTest\\TestFiles\\RINEX_nav\\mobs2530.17n");

		RinexSatID sat;

		//Open the files
		istrm.open(filepath_obs, ios::in);
		inavstrm.open(filepath_nav.c_str(), ios::in);

		if (!istrm.is_open() || !inavstrm.is_open()) {
			LOG(WARNING) << "Warning : could not open file ";
			iret = 1;
		}
		try
		{
			std::cout << "Parsing Started. Wait for [ERROR] or a [FLAG:...] message." << endl;
			istrm >> Rhead;
			inavstrm >> Rnavhead;

			try
			{
				indexC1 = Rhead.getObsIndex("C1");	//Pseudorange obs index
			}
			catch (...)
			{
				exit(1);
			}

			// Store Ephemeris Data
			while (inavstrm >> Rnavdata) {
				bceStore.addEphemeris(Rnavdata);
			}

			while (istrm >> Rdata) {
				CivilTime civtime(Rdata.time);
				Rinex3ObsData::DataMap::const_iterator it;

				// Iterate over observations
				for (it = Rdata.obs.begin(); it != Rdata.obs.end(); it++) {
					double C1(0.0);
					try
					{
						sat = (*it).first; //Get Sat ID
						//		Skip if Sat is not GPS		Skip if Observations are less than 7
						if (SatID::systemGPS != sat.system || (*it).second.capacity() != 7) continue;
						C1 = Rdata.getObs((*it).first, indexC1).data;			//Get C1 Pseudorange observation
						xvt_data = bceStore.getXvt((*it).first, civtime);		//Get XVT data

						//satDataContainer_c.assembleTrajectories(sat, civtime, xvt_data,C1);	//Pass data to storage interface
						satDataContainer_c.assemblePseudoRangeContainer(sat, civtime, C1);
						//cout << civtime << " " << sat << " " << C1 << " XVT: " << xvt_data << endl;	// TODO: delete later (debug cout)

					}
					catch (...)
					{
						std::cout << "WARNING: C1 not found for SV:   " << sat << " at epoch:   " << civtime << endl;
					}
				}
			}
			//Add EphemerisStore to Container class
			satDataContainer_c.passEphemerisStore(bceStore);
			std::cout << "[FLAG: Success] Finished Rinex parsing." << endl;
		}
		catch (const std::exception& e)
		{

		}

		return 0;
	}
	catch (...) {

		return 1;
	}
}

int ProcessTrajectoryFile(void){

	gnsssimulator::TrajectoryStream trajFileIn("..\\Simulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord.txt");
	gnsssimulator::TrajectoryHeader trajHeader;
	gnsssimulator::TrajectoryData trajData;

	trajFileIn >> trajHeader;
	
	while (trajFileIn >> trajData) {
		trajStore.addPosition(trajData);
	}
	std::cout << "[FLAG: Success] Trajectory file parsing finished." << endl;
	return 0;
}

bool Test_ReProcess_Solution_Rinex(void)
{
	bool test_success = 1;

	Rinex3ObsStream stream;
	Rinex3ObsHeader head;
	Rinex3ObsData data;
	satDataContainer satDataCont_Test;
	SatID satid;
	double pr;
	stream.open("..\\Simulator\\TrajectoryTestFiles\\generatedRINEX.11o", ios::in);

	if (!stream.is_open())
		!test_success;
	try
	{
		stream >> head;
		int indexC1 = head.getObsIndex("C1");

		while (stream >> data) {
			CivilTime civtime(data.time);
			for (auto& it : data.obs) {
				satid = it.first;
				pr = data.getObs(satid, indexC1).data;
				satDataCont_Test.assemblePseudoRangeContainer(satid, civtime, pr);
			}
		}
	}
	catch (...)
	{
		test_success = 0;
	}
	try
	{
		double test_pr, original_pr;
		CivilTime civtime;
		for (auto& satid_it : satDataCont_Test.getSatIDvectorlist()) {
			for (auto& time_it : satDataCont_Test.getEpochVectorforSat(satid_it)) {
				test_pr = satDataCont_Test.getPseudorangeatEpoch(satid_it, time_it);
				original_pr = satDataContainer_c.getPseudorangeatEpoch(satid_it, time_it);
				if (test_pr != original_pr)
					test_success = 0;
			}
			
		}
	}
	catch (...)
	{
		test_success = 0;
	}

	return test_success;
}
