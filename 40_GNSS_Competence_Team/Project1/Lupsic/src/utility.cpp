
#include "utility.h"
#include "simpleNavigationSolution.h"

static GPSEphemerisStore bcestore;

void set_bcestore(GPSEphemerisStore in_bcestore) {

	bcestore = in_bcestore;
}

void setSimpleNaviagtionCalculator(gpstk::CommonTime time, vector<SatID> vid, vector<double> prv) {

	GPSWeekSecond gpstime(time);

	int vsize = 0;

	for (int i = 0; i < vid.size(); i++) {
		if (vid[i].system == SatID::systemGPS) {
			vsize++;
		}
	}
	int* satId;
	double* pr;

	satId = (int*)malloc(sizeof(int)*vsize);
	pr = (double*)malloc(sizeof(double)*vsize);

	for (int i = 0; i < vsize; i++) {
		if (vid[i].system == SatID::systemGPS){
			satId[i] = vid[i].id;
			pr[i] = prv[i];
		}
	}

	set_time(gpstime.week, gpstime.sow);
	set_satId(satId, vsize);
	set_pseudoRange(pr, vsize);
}

int get_satPos(int wn, double tow, int satId, double* pos) {

	SatID id;
	id.id = satId;
	id.system = SatID::SatelliteSystem::systemGPS;

	GPSWeekSecond gpstime;
	gpstime.week = wn;
	gpstime.sow = tow;

	Xvt xvt;

	try {

		xvt = bcestore.getXvt(id, gpstime);
		pos[0] = xvt.x[0];
		pos[1] = xvt.x[1];
		pos[2] = xvt.x[2];

		return 1;
	}
	catch (Exception& e){

		pos[0] = 0;
		pos[1] = 0;
		pos[2] = 0;

		//cerr << e << endl;

		return 0;
	}
}

int get_satClock(int wn, double tow, int satId, double &clockbias) {

	SatID id;
	id.id = satId;
	id.system = SatID::SatelliteSystem::systemGPS;

	GPSWeekSecond gpstime;
	gpstime.week = wn;
	gpstime.sow = tow;

	Xvt xvt;

	try {

		xvt = bcestore.getXvt(id, gpstime);
		clockbias = xvt.getClockBias();

		return 1;
	}
	catch (Exception& e) {

		//cerr << e << endl;

		return 0;
	}

}

int get_satRelCorr(int wn, double tow, int satId, double &relcorr) {

	SatID id;
	id.id = satId;
	id.system = SatID::SatelliteSystem::systemGPS;

	GPSWeekSecond gpstime;
	gpstime.week = wn;
	gpstime.sow = tow;

	Xvt xvt;

	try {

		xvt = bcestore.getXvt(id, gpstime);
		relcorr = xvt.relcorr;

		return 1;
	}
	catch (Exception& e) {

		//cerr << e << endl;

		return 0;
	}
}