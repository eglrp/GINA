
//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  Copyright 2009, The University of Texas at Austin
//
//============================================================================

// GPSTk example program #4

// Project 2. DGNSS Navigation Calculator
#include "DGNSSNavigationSolution.h"
#include "utility.h"

using namespace std;
using namespace gpstk;

static void print_Solution(const int, const double, const double[3], const double[3], const double[3], const double[3]);
void setPRNVectorandSatIdVect(const Rinex3ObsData&, const int&, const int&, vector<SatID>&, vector<double>&);

int main(void)
{
	string RoverObsFilewPath = "..\\..\\..\\..\\40_GNSS_Competence_Team\\Project2\\data\\holp2620.17o";
	string BaseObsFilewPath = "..\\..\\..\\..\\40_GNSS_Competence_Team\\Project2\\data\\lbch2620.17o";
	string BrdEphFilewPath = "..\\..\\..\\..\\40_GNSS_Competence_Team\\Project2\\data\\brdc2620.17n";
	double basePosition[3] = {  -2507798.7984,
								-4676369.6918,
								 3526890.8008 };
	double referenceRoverPosition[3] = {	-2500946.0962,
											-4670472.9515,
											 3539500.5469 };
	const double latDegree = 33.5528;
	const int day = 75;

	double GPSTKsolutionRoverPosition[3];
	double GPSTKsolutionRoverPositionIonoFree[3];
	double DGNSSsolutionRoverPosition[3];

		/*lbch site*/
		/*City or Town				: Long Beach
		State or Province			: California
		Country						: United States
		Tectonic Plate				: NORTH AMERICAN
		Approximate Position(ITRF)
			X coordinate(m)			: -2507798.7984
			Y coordinate(m)			: -4676369.6918
			Z coordinate(m)			: 3526890.8008
			Latitude(N is + )		: +334715.97
			Longitude(E is + )		: -1181212.04
			Elevation(m, ellips.)	: -27.58
		Additional Information		: Los Angeles County.*/
		
		/*holp*/
		/*City or Town				: Hollydale
		State or Province			: California
		Country						: United States of America
		Tectonic Plate				: PACIFIC
		Approximate Position(ITRF)
			X coordinate(m)			: -2500946.0962
			Y coordinate(m)			: -4670472.9515
			Z coordinate(m)			: 3539500.5469
			Latitude(N is + )		: +335528.34
			Longitude(E is + )		: -1181005.41
			Elevation(m, ellips.)	: -6.71
		Additional Information		: Los Angeles County.*/

	// Declaration of objects for storing ephemerides and handling RAIM
	GPSEphemerisStore bcestore; // This is now static
	PRSolution2 raimSolver;

	// Object for void-type tropospheric model (in case no meteorological
	// RINEX is available)
	ZeroTropModel noTropModel;

	// Object for GG-type tropospheric model (Goad and Goodman, 1974)
	// Default constructor => default values for model
	GGTropModel ggTropModel;
	SaasTropModel ssTropModel(latDegree,day);
	ssTropModel.setWeather(23,1013.25,70);

	// Pointer to one of the two available tropospheric models. It points
	// to the void model by default
	TropModel *tropModelPtr = &ssTropModel;

	try
	{

		// Read nav file and store unique list of ephemerides
		Rinex3NavStream rnffs(BrdEphFilewPath.c_str());    // Open ephemerides data file
		Rinex3NavData rne;
		Rinex3NavHeader hdr;

		// Let's read the header (may be skipped)
		rnffs >> hdr;

		// Storing the ephemeris in "bcstore"
		while (rnffs >> rne) { 
			try { 
				bcestore.addEphemeris(rne); 
			}
			catch(Exception& e){
				cerr << e;
			}
			
		}

		// Setting the criteria for looking up ephemeris
		bcestore.SearchNear();
		set_bcestore(bcestore);

		   // Open and read the observation file one epoch at a time.
		   // For each epoch, compute and print a position solution

		Rinex3ObsStream roffsBase(BaseObsFilewPath.c_str());    // Open observations data file
		Rinex3ObsStream roffsRover(RoverObsFilewPath.c_str());    // Open observations data file

										   // In order to throw exceptions, it is necessary to set the failbit
		roffsBase.exceptions(ios::failbit);
		roffsRover.exceptions(ios::failbit);

		Rinex3ObsHeader rohRover;
		Rinex3ObsHeader rohBase;
		Rinex3ObsData rodRover;
		Rinex3ObsData rodBase;

		// Let's read the header
		roffsBase >> rohBase;
		roffsRover >> rohRover;

		// The following lines fetch the corresponding indexes for some
		// observation types we are interested in. Given that old-style
		// observation types are used, GPS is assumed.
		int indexC1Base, indexC1Rover;
		try
		{
			indexC1Rover = rohRover.getObsIndex("C1");
			indexC1Base = rohBase.getObsIndex("C1");
		}
		catch (...)
		{
			cerr << "The observation file doesn't have C1 pseudoranges." << endl;
			exit(1);
		}

		int indexC2Rover;
		try
		{
			indexC2Rover = rohRover.getObsIndex("C2");
		}
		catch (...)
		{
			cerr << "The observation file doesn't have P2 pseudoranges." << endl;
			indexC2Rover = -1;
		}

		// Let's process one line of observation data
		while (roffsBase >> rodBase && roffsRover >> rodRover) // if you would you like to read all the line use this.
		//roffs >> rod;
		{
			   // Apply editing criteria
			if (rodBase.epochFlag == 0	|| rodBase.epochFlag == 1 ||
				rodRover.epochFlag == 0 || rodRover.epochFlag == 1 )  // Begin usable data
			{

				vector<SatID> prnVecRover, prnVecRoverIonoFree;
				vector<double> rangeVecRover, rangeVecRoverIonoFree;
				vector<SatID> prnVecBase;
				vector<double> rangeVecBase;

				// This part gets the PRN numbers and 
				// pseudoranges for the current epoch. They are correspondly fed
				// into "prnVec" and "rangeVec"; "obs" is a public attribute of
				// Rinex3ObsData to get the map of observations

				setPRNVectorandSatIdVect(rodBase, indexC1Base, -1, prnVecBase, rangeVecBase);
				setPRNVectorandSatIdVect(rodRover, indexC1Rover, -1, prnVecRover, rangeVecRover);
				setPRNVectorandSatIdVect(rodRover, indexC1Rover, 3, prnVecRoverIonoFree, rangeVecRoverIonoFree);

				   // The default constructor for PRSolution2 objects (like
				   // "raimSolver") is to set a RMSLimit of 6.5. We change that
				   // here. With this value of 3e6 the solution will have a lot
				   // more dispersion.
				raimSolver.RMSLimit = 3e6;

				// Simple solution without RAIM
				raimSolver.NSatsReject = 0;

				// In order to compute positions we need the current time, the
				// vector of visible satellites, the vector of corresponding
				// ranges, the object containing satellite ephemerides, and a
				// pointer to the tropospheric model to be applied
				raimSolver.RAIMCompute(	rodRover.time,
										prnVecRoverIonoFree,
										rangeVecRoverIonoFree,
										bcestore,
										tropModelPtr);

				GPSTKsolutionRoverPositionIonoFree[0] = raimSolver.Solution[0];
				GPSTKsolutionRoverPositionIonoFree[1] = raimSolver.Solution[1];
				GPSTKsolutionRoverPositionIonoFree[2] = raimSolver.Solution[2];

				raimSolver.RAIMCompute(	rodRover.time,
										prnVecRover,
										rangeVecRover,
										bcestore,
										tropModelPtr);

				GPSTKsolutionRoverPosition[0] = raimSolver.Solution[0];
				GPSTKsolutionRoverPosition[1] = raimSolver.Solution[1];
				GPSTKsolutionRoverPosition[2] = raimSolver.Solution[2];

				// Note: Given that the default constructor sets public
				// attribute "Algebraic" to FALSE, a linearized least squares
				// algorithm will be used to get the solutions.
				// Also, the default constructor sets ResidualCriterion to true,
				// so the rejection criterion is based on RMS residual of fit,
				// instead of RMS distance from an a priori position.

				// If we got a valid solution, let's print it

				if (raimSolver.isValid())
				{
					// Vector "Solution" holds the coordinates, expressed in
					// meters in an Earth Centered, Earth Fixed (ECEF) reference
					// frame. The order is x, y, z  (as all ECEF objects)
					/*cout << setprecision(12) << raimSolver.Solution[0] << " ";
					cout << raimSolver.Solution[1] << " ";
					cout << raimSolver.Solution[2];
					cout << endl;*/

				}  // End of 'if( raimSolver.isValid() )'


				  ///////////////////////////////////////////
				 // Differential GNSS navigation solution //
				///////////////////////////////////////////
				// Set parameters
				setDGNSSNaviagtionCalculator_Rover(rodRover.time, prnVecRover, rangeVecRover);
				setDGNSSNaviagtionCalculator_Base(rodBase.time, prnVecBase, rangeVecBase, basePosition);

				// Calculate position
				calculatePosition();

				// Print result
				cout << endl;
				cout << "Next epoch" << endl;

				get_Pos_Result(DGNSSsolutionRoverPosition);

				

				GPSWeekSecond gpstime(rodRover.time);
				print_Solution(gpstime.week, gpstime.sow, referenceRoverPosition, GPSTKsolutionRoverPosition, GPSTKsolutionRoverPositionIonoFree, DGNSSsolutionRoverPosition);
				cout << endl;
				
			} // End of 'if( rod.epochFlag == 0 || rod.epochFlag == 1 )'

		}  // End of 'while( roffs >> rod )'

	}
	catch (Exception& e)
	{
		cerr << e << endl;
	}
	catch (...)
	{
		cerr << "Caught an unexpected exception." << endl;
	}

	exit(0);

}
static void print_Solution(const int WN, const double ToW, const double referenceRoverPosition[3], const double GPSTKsolutionRoverPosition[3], const double GPSTKsolutionRoverPositionIonoFree[3], const double DGNSSsolutionRoverPosition[3]) {

	double absDifferenGPSTK_Ref;
	double absDifferenDGNSS_Ref;
	double absDifferenGPSTKIonoFree_Ref;

	cout << setprecision(12);
	cout << "Time: " << WN << " " << ToW << endl;
	cout << "GPSTK solution: " << endl;
	cout << GPSTKsolutionRoverPosition[0] << " ";
	cout << GPSTKsolutionRoverPosition[1] << " ";
	cout << GPSTKsolutionRoverPosition[2];
	cout << endl;

	cout << "DGNSS solution: " << endl;
	cout << DGNSSsolutionRoverPosition[0] << " ";
	cout << DGNSSsolutionRoverPosition[1] << " ";
	cout << DGNSSsolutionRoverPosition[2];
	cout << endl;

	cout << "GPSTK-Reference solution: " << endl;
	cout << GPSTKsolutionRoverPosition[0] - referenceRoverPosition[0] << " ";
	cout << GPSTKsolutionRoverPosition[1] - referenceRoverPosition[1] << " ";
	cout << GPSTKsolutionRoverPosition[2] - referenceRoverPosition[2];
	cout << endl;

	cout << "GPSTK Iono Free-Reference solution: " << endl;
	cout << GPSTKsolutionRoverPositionIonoFree[0] - referenceRoverPosition[0] << " ";
	cout << GPSTKsolutionRoverPositionIonoFree[1] - referenceRoverPosition[1] << " ";
	cout << GPSTKsolutionRoverPositionIonoFree[2] - referenceRoverPosition[2];
	cout << endl;

	
	cout << "DGNSS-Reference solution: " << endl;
	cout << DGNSSsolutionRoverPosition[0] - referenceRoverPosition[0] << " ";
	cout << DGNSSsolutionRoverPosition[1] - referenceRoverPosition[1] << " ";
	cout << DGNSSsolutionRoverPosition[2] - referenceRoverPosition[2];
	cout << endl;

	absDifferenDGNSS_Ref = sqrt(pow((DGNSSsolutionRoverPosition[0] - referenceRoverPosition[0]), 2) +
								pow((DGNSSsolutionRoverPosition[1] - referenceRoverPosition[1]), 2) +
								pow((DGNSSsolutionRoverPosition[2] - referenceRoverPosition[2]), 2));

	absDifferenGPSTK_Ref = sqrt(pow((GPSTKsolutionRoverPosition[0] - referenceRoverPosition[0]), 2) +
								pow((GPSTKsolutionRoverPosition[1] - referenceRoverPosition[1]), 2) +
								pow((GPSTKsolutionRoverPosition[2] - referenceRoverPosition[2]), 2));

	absDifferenGPSTKIonoFree_Ref = sqrt(pow((GPSTKsolutionRoverPositionIonoFree[0] - referenceRoverPosition[0]), 2) +
										pow((GPSTKsolutionRoverPositionIonoFree[1] - referenceRoverPosition[1]), 2) +
										pow((GPSTKsolutionRoverPositionIonoFree[2] - referenceRoverPosition[2]), 2));

	cout << endl;
	cout << "Absolute error w GPSTK				: " << absDifferenGPSTK_Ref << endl;
	cout << "Absolute error w GPSTK Iono Free	: " << absDifferenGPSTKIonoFree_Ref << endl;
	cout << "Absolute error w DGNSS				: " << absDifferenDGNSS_Ref << endl;
	cout << endl;

}

void setPRNVectorandSatIdVect(const Rinex3ObsData& rod, const int &indexC1, const int &indexC2, vector<SatID> &prnVec, vector<double> &rangeVec) {

	// Let's compute an useful constant (also found in "GNSSconstants.hpp")
	const double gamma = (L1_FREQ_GPS / L2_FREQ_GPS)*(L1_FREQ_GPS / L2_FREQ_GPS);

	// Define the "it" iterator to visit the observations PRN map. 
	// Rinex3ObsData::DataMap is a map from RinexSatID to
	// vector<RinexDatum>:
	//      std::map<RinexSatID, vector<RinexDatum> >
	Rinex3ObsData::DataMap::const_iterator it;

	// This part gets the PRN numbers and
	// pseudoranges for the current epoch. They are correspondly fed
	// into "prnVec" and "rangeVec"; "obs" is a public attribute of
	// Rinex3ObsData to get the map of observations
	for (it = rod.obs.begin(); it != rod.obs.end(); it++)
	{

		// The RINEX file may have P1 observations, but the current
		// satellite may not have them.
		double P1(0.0);
		try
		{
			P1 = rod.getObs((*it).first, indexC1).data;
		}
		catch (...)
		{
			// Ignore this satellite if P1 is not found
			continue;
		}


		double ionocorr(0.0);

		// If there are P2 observations, let's try to apply the
		// ionospheric corrections
		if (indexC2 >= 0)
		{

			// The RINEX file may have P2 observations, but the
			// current satellite may not have them.
			double P2(0.0);
			try
			{
				P2 = rod.getObs((*it).first, indexC2).data;
			}
			catch (...)
			{
				// Ignore this satellite if P1 is not found
				continue;
			}

			// Vector 'vecData' contains RinexDatum, whose public
			// attribute "data" indeed holds the actual data point
			ionocorr = 1.0 / (1.0 - gamma) * (P1 - P2);

		}

		// Now, we include the current PRN number in the first part
		// of "it" iterator into the vector holding the satellites.
		// All satellites in view at this epoch that have P1 or P1+P2
		// observations will be included.
		prnVec.push_back((*it).first);

		// The same is done for the vector of doubles holding the
		// corrected ranges
		rangeVec.push_back(P1 - ionocorr);

		// WARNING: Please note that so far no further correction
		// is done on data: Relativistic effects, tropospheric
		// correction, instrumental delays, etc.

	}  // End of 'for( it = rod.obs.begin(); it!= rod.obs.end(); ...'
}