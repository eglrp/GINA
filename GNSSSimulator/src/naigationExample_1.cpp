#pragma

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
//  Copyright 2006, The University of Texas at Austin
//
//============================================================================

#include "stdafx.h"
#include "Navigation_examples.h"

#include "RinexNavBase.hpp"
#include "RinexNavData.hpp"
#include "RinexNavHeader.hpp"
#include "RinexNavStream.hpp"

using namespace std;


void Navigation_examples_1(void)
{

   try
		   {
	   std::string readfileNamewPath("C:\\Users\\LUS2BP\\Source\\Repos\\GPSTk-master\\examples\\bahr1620.04n");
	   std::string writefileNamewPath("C:\\Users\\LUS2BP\\Source\\Repos\\GPSTk-master\\examples\\bahr1620_write.04n");
		      cout << "Reading " << readfileNamewPath << endl;
		      gpstk::RinexNavStream rnffs(readfileNamewPath.c_str());
		      cout << "Writing " << writefileNamewPath << endl;
		      gpstk::RinexNavStream out(writefileNamewPath.c_str(), ios::out);
		      gpstk::RinexNavHeader rnh;
		      gpstk::RinexNavData rne;
			 
			  rnffs >> rnh;
		      out << rnh;
		      rnh.dump(cout);
		
			        int i = 0;
		      while (rnffs >> rne)
			      {
			         out << rne;
			         i++;
			      }
					
			  cout << "Read " << i << " records.  Done." << endl;
			  return;
		    }
	    catch (gpstk::Exception& e)
		    {
		       cout << e;
			   return;
		    }
	    catch (...)
		    {
		       cout << "unknown error.  Done." << endl;
		       return;
		    }
	
		    return;
	 } 