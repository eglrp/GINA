#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <math.h>

#include "std_IMUSimulator.h"

#ifndef DEG2RAD
#define DEG2RAD 0.017453292519943295769222
#endif // !DEG2RAD

namespace IMUSimulator {


	class WGS84Ellipsoid {

		private: 
			double WIE_E;
			double SM_AXIS;
			double E_SQR;
			double NORMAL_GRV;
			double GRV_CONS;
			double FLATTENING;
			double M_FAKTOR;

		public:
			double llh[3];

			/*Constructors*/
			WGS84Ellipsoid(void);
			WGS84Ellipsoid(double[], CoordiateFrame);
			WGS84Ellipsoid::WGS84Ellipsoid(const WGS84Ellipsoid&);

			WGS84Ellipsoid& operator=(const WGS84Ellipsoid& wgs84);

			// LLH in degree ECEF in m
			void setCoordinates(double[], CoordiateFrame);
			void getParams(double&, double&, double&, double&, double&, double&);
			void get_Gravity_and_WIE_E(double&, double&);

	private:
		void setWGS84Params(void);

	};


}