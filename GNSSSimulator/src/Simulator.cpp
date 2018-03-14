// Simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Simulator.h"
#include "Navigation_examples.h"
#include "Test_Trajectory_Class.hpp"
#include "Test_simulatePseudoRange.hpp"
#include "test_PseudoRangeCalculator.h"

const int ProcessRinex = 1;
const int ProcessTrajectory = 1;
const int Solution_to_RINEX = 1;
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

/// Normal Distribution Error
std::default_random_engine generator(time(0));
std::normal_distribution<double> dist(0.0, 0.01);		// Needs an initial non-NULL value, else throws an exception, we can set a proper value later in the program.

int _tmain(int argc, _TCHAR* argv[]) {

	//PseudoRangeCalculator_test1();
	//PseudoRangeCalculator_test2();
	//PseudoRangeCalculator_test3();
	//PseudoRangeCalculator_test4();
	//PseudoRangeCalculator_test5();
	//PseudoRangeCalculator_test6();
	PseudoRangeCalculator_test7();
	return 0;
}

/*int _tmain(int argc, _TCHAR* argv[])
{
	cout << "CONFIG: " << "ProcessRinex " << ProcessRinex << " ProcessTrajectory: " << ProcessTrajectory <<
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
	/*cout << endl << endl << "------------" << endl;
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
	//bceStore.dump(cout, 3);

	// Calculate base pseudoranges and assign them to the container
	#pragma region Pseudorange 0th Solution
	vector<GPSWeekSecond>traj_timevec = trajStore.listTime();
	double Prange;

	SatDataEpoch satDataEpoch;												// Sat Data map<SatID,SatPosition>
	SolutionDataBlock solutionDataBlock;									//+RoverPos : Pair<Triple,SatDataEpoch>
	gnsssimulator::PRsolution::PRSolutionContainer prsolutionContainer;		// The whole solution container
	vector<SatID> goodSatVector;
	double elevation = 0.0;														// Elevation value of Sat from Rover
	

	for (auto& it : traj_timevec) {
		CivilTime civtime = it.convertToCommonTime();
		cout << setw(30) << endl;
		cout << "Next Epoch: " << endl;
		cout << it << "  converted to civtime: " << civtime << endl;
		/// Rover Position
		gnsssimulator::TrajectoryData data = trajStore.findPosition(it);
		cout << "Rover Position:     " << data.pos << endl << endl;

		satDataEpoch.clear();

		for (auto& satid_it : satDataContainer_c.getSatIDvectorlist()) { // TODO modify to get satvectorlist from NAV rinex instead of OBS rinex
			try
			{
				Xvt xvt_data;
				try
				{
					CivilTime civtime_temp = civtime;
					

					xvt_data = bceStore.getXvt(satid_it, civtime);
					Prange = prsolution.getPRSolution_abs(data.pos, xvt_data.x);	
					
					/// Iterative Satellite Position Solution
					for (int i = 0;i < 3;i++) {
						double t_transmission = Prange / prsolution.C_light;

						/// Handle civtime_temp with rollover		
						civtime_temp.second = civtime.second - t_transmission;

						if (civtime_temp.second <= 0) {
							civtime_temp.minute -= 1;
							civtime_temp.second = 60.0 + civtime_temp.second;		// + because second is negative here
							if (civtime_temp.minute <= 0) {		//Hour rollover
								civtime_temp.hour -= 1;
								civtime_temp.minute = 60 + civtime_temp.minute;		//Same as above
								if (civtime_temp.hour <= 0) {	//Day rollover 
									civtime_temp.day -= 1;
									civtime_temp.hour = 24 + civtime_temp.hour;
								}
							}
						}
						
							///Get new XVT Position data
							xvt_data = bceStore.getXvt(satid_it, civtime_temp);
							///Calculate elevation angle from rover position 
							Position SatPos(xvt_data), SitePos(data.pos);
							elevation = SitePos.elevation(SatPos);
							if (elevation <= 25.0)
								throw std::invalid_argument(" Elevation is below threshold.");
							///Calculate new Prange
							Prange = prsolution.getPRSolution_abs(data.pos, xvt_data.x);
							//cout << "pr : " << Prange << endl;
							
					
					}
					/// End of PR Iteration
					
					satDataEpoch[satid_it] = xvt_data.x;

					cout << " Sat ID: " << satid_it << " Pseudorange: " << Prange <<
						" Signal tt: " << prsolution.getSignal_tt()
						<< " Elevation: " << elevation << endl;

				}
				catch (...)	//Satellites that do not have OrbitEph - we don't need those - throw Exception from getXvt()
				{			
					
				}
				
				
			}
			catch (...)		//const gpstk::InvalidRequest& e
			{				
				//cout << "[Warning] Can't get OrbitEph for " << satid_it << " at: " << civtime << endl;
			}
		}
		solutionDataBlock.first = data.pos;
		solutionDataBlock.second = satDataEpoch;
		prsolutionContainer[civtime] = solutionDataBlock;
	}

	for (auto &x: satDataEpoch)		//Create the good sat vector that only contains SatID with valid OrbitEphs
	{
		goodSatVector.push_back(x.first);

	}
#pragma endregion

	//Calculate Site position using built-in RAIM
#pragma region PseudoRange RaimCompute Solution
	ofstream ostrm("..\\Simulator\\TrajectoryTestFiles\\output_RaimSolution.txt", std::ios::out);	//Output file
	ofstream ostrm_log("..\\Simulator\\Log\\RAIM_pre_LOG.txt", std::ios::out); // Outut LOG
	PRSolution2 RaimSolver;
	//PRSolution RaimSolver1;
	
	vector<double> prvector;
	ZeroTropModel zeroTrop;
	TropModel *tropModelPtr = &zeroTrop;
	CivilTime correctedCivtime; // TODO nevezek fura, felrevezeto
	for (auto& it : traj_timevec) {
		CivilTime civtime = it.convertToCommonTime();
		/// Civtime Log
		ostrm_log << civtime << endl;

		prvector.clear();
		Xvt xvt_data;	//For error correction
		double errorcorr;
		for (auto& satid_it : satDataEpoch) { // satDataContainer_c.getSatIDvectorlist()	// TODO satdataepoch nem jó

				double pr_obs;
				double pr_calc;
				//const SatID &currSat = satid_it.first;

				Triple roverpos = prsolutionContainer[civtime].first;
				Triple satPos = prsolutionContainer[civtime].second[satid_it.first];
				pr_calc = prsolution.getPRSolution_abs(roverpos,satPos);	// TODO: Might cause the 40km issue
				//pr_calc = satDataContainer_c.getPseudorangeatEpoch(satid_it.first, civtime);

				try		// TODO: obs rinex pr is not needed
				{
					//pr_obs = satDataContainer_c.getPseudorangeatEpoch(satid_it.first, civtime);
				}
				catch (const std::exception&)
				{
					pr_obs = 0.0;
				}

				prvector.push_back(pr_calc);

				///Logging
				
				ostrm_log << satid_it.first << "     Pseudorange:  " << setprecision(16) << pr_calc << endl;

			}
			
		cout << "Log created." << endl;
		
		cout << "RaimCompute started." << endl;
		cout << RaimSolver.RAIMCompute(civtime, goodSatVector, prvector, bceStore, tropModelPtr) << endl;
		cout << std::setprecision(12) << RaimSolver.Solution[0] << " " <<
			std::setprecision(12) << RaimSolver.Solution[1] << "  " <<
			std::setprecision(12) << RaimSolver.Solution[2] << endl;
		ostrm << std::setprecision(12) << RaimSolver.Solution[0] << " " << std::setprecision(12) << RaimSolver.Solution[1]
			<< " " << std::setprecision(12) << RaimSolver.Solution[2] << endl;
		cout << "Size of sat vector:" << goodSatVector.size() << endl;
		cout << "Number of godd satelite are used in the solution: " << RaimSolver.Nsvs << endl;
		
	}
	ostrm.close();
#pragma endregion

	if (Solution_to_RINEX) {
		cout << "Creating Rinex File. " << endl;
		prsolution.prepareRinexData(prsolutionContainer);
		prsolution.createRinexFile();
	}

	if (Run_Tests) {
		cout << "Solution Reprocess ?= Original Rinex Process: " << Test_ReProcess_Solution_Rinex() << endl;
	}
	

	return 0;
}*/

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
			cout << "Parsing Started. Wait for [ERROR] or a [FLAG:...] message." << endl;
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
				for (it = Rdata.obs.begin();it != Rdata.obs.end();it++) {
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
						cout << "WARNING: C1 not found for SV:   " << sat << " at epoch:   " << civtime << endl;
					}
				}
			}
			//Add EphemerisStore to Container class
			satDataContainer_c.passEphemerisStore(bceStore);
			cout << "[FLAG: Success] Finished Rinex parsing." << endl;
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

	gnsssimulator::TrajectoryStream trajFileIn("..\\Simulator\\TrajectoryTestFiles\\TrajectoryFileExample_RinexMatch_rinexcoord_long.txt");
	gnsssimulator::TrajectoryHeader trajHeader;
	gnsssimulator::TrajectoryData trajData;

	trajFileIn >> trajHeader;
	
	while (trajFileIn >> trajData) {
		trajStore.addPosition(trajData);
	}
	cout << "[FLAG: Success] Trajectory file parsing finished." << endl;
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
