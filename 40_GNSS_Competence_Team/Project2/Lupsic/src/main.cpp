
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

// Project 1. Simple Navigation Calculator
#include "DGNSSNavigationSolution.h"
#include "utility.h"

using namespace std;
using namespace gpstk;

void setPRNVectorandSatIdVect(const Rinex3ObsData&, const int&, vector<SatID>&, vector<double>&);

int main(void)
{
	string RoverObsFilewPath = "..\\..\\..\\..\\40_GNSS_Competence_Team\\Project2\\data\\holp2620.17o";
	string BaseObsFilewPath = "..\\..\\..\\..\\40_GNSS_Competence_Team\\Project2\\data\\lbch2620.17o";
	string BrdEphFilewPath = "..\\..\\..\\..\\40_GNSS_Competence_Team\\Project2\\data\\brdc2620.17n";
	double basePosition[3] = {  -2507798.7984,
								-4676369.6918,
								 3526890.8008 };
		
		/*lbch site*/
		/*City or Town             : Long Beach
		State or Province : California
		Country : United States
		Tectonic Plate : NORTH AMERICAN
		Approximate Position(ITRF)
		X coordinate(m) : -2507798.7984
		Y coordinate(m) : -4676369.6918
		Z coordinate(m) : 3526890.8008
		Latitude(N is + ) : +334715.97
		Longitude(E is + ) : -1181212.04
		Elevation(m, ellips.) : -27.58
		Additional Information : Los Angeles County.*/

	// Declaration of objects for storing ephemerides and handling RAIM
	GPSEphemerisStore bcestore; // This is now static
	PRSolution2 raimSolver;

	// Object for void-type tropospheric model (in case no meteorological
	// RINEX is available)
	ZeroTropModel noTropModel;

	// Object for GG-type tropospheric model (Goad and Goodman, 1974)
	// Default constructor => default values for model
	GGTropModel ggTropModel;

	// Pointer to one of the two available tropospheric models. It points
	// to the void model by default
	TropModel *tropModelPtr = &noTropModel;

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
		int indexP1Base, indexP1Rover;
		try
		{
			indexP1Rover = rohRover.getObsIndex("C1");
			indexP1Base = rohBase.getObsIndex("C1");
		}
		catch (...)
		{
			cerr << "The observation file doesn't have P1 pseudoranges." << endl;
			exit(1);
		}

		// Let's process one line of observation data
		while (roffsBase >> rodBase && roffsRover >> rodRover) // if you would you like to read all the line use this.
		//roffs >> rod;
		{
			   // Apply editing criteria
			if (rodBase.epochFlag == 0	|| rodBase.epochFlag == 1 ||
				rodRover.epochFlag == 0 || rodRover.epochFlag == 1 )  // Begin usable data
			{

				vector<SatID> prnVecRover;
				vector<double> rangeVecRover;
				vector<SatID> prnVecBase;
				vector<double> rangeVecBase;

				// This part gets the PRN numbers and 
				// pseudoranges for the current epoch. They are correspondly fed
				// into "prnVec" and "rangeVec"; "obs" is a public attribute of
				// Rinex3ObsData to get the map of observations

				setPRNVectorandSatIdVect(rodBase, indexP1Base, prnVecBase, rangeVecBase);
				setPRNVectorandSatIdVect(rodRover, indexP1Rover, prnVecRover, rangeVecRover);

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
										prnVecRover,
										rangeVecRover,
										bcestore,
										tropModelPtr);

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
					cout << setprecision(12) << raimSolver.Solution[0] << " ";
					cout << raimSolver.Solution[1] << " ";
					cout << raimSolver.Solution[2];
					cout << endl;

				}  // End of 'if( raimSolver.isValid() )'


				////////////////////////////////
				// Differential GNSS navigation solution //
				////////////////////////////////
				// Set parameters
				setDGNSSNaviagtionCalculator_Rover(rodRover.time, prnVecRover, rangeVecRover);
				setDGNSSNaviagtionCalculator_Base(rodBase.time, prnVecBase, rangeVecBase, basePosition);

				// Calculate position
				calculatePosition();

				// Print result
				print_Result();

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

void setPRNVectorandSatIdVect(const Rinex3ObsData& rod, const int &indexP1, vector<SatID> &prnVec, vector<double> &rangeVec) {

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
			P1 = rod.getObs((*it).first, indexP1).data;
		}
		catch (...)
		{
			// Ignore this satellite if P1 is not found
			continue;
		}

		// Now, we include the current PRN number in the first part
		// of "it" iterator into the vector holding the satellites.
		// All satellites in view at this epoch that have P1 or P1+P2
		// observations will be included.
		prnVec.push_back((*it).first);

		// The same is done for the vector of doubles holding the
		// corrected ranges
		rangeVec.push_back(P1);

		// WARNING: Please note that so far no further correction
		// is done on data: Relativistic effects, tropospheric
		// correction, instrumental delays, etc.

	}  // End of 'for( it = rod.obs.begin(); it!= rod.obs.end(); ...'
}