#pragma once
#include <fstream>
#include <string>
#include <iostream>
//#include "GNSStime.h"
#include "CoordinateFrames.h"
#include "CoordinateFrameHandler.h"

#define TESTING

class trajectoryReader
{


public:
	trajectoryReader();
	trajectoryReader(char*);
	~trajectoryReader();

	void open(void);
	void close(void);
	FULLFrame readLine_as_frame(void);
	

#ifdef TESTING

public:

#else

private:

#endif
	
	GPSTime nextTime(void);
	LLHCoordinate nextLLH(void);
	ECEFCoordinate nextECEF(void);
	bool isFormatValid(void);
	bool is_open();
	void setFile2Begining(void);
	std::string readHeader();
	std::string readLine(void);

private:
	void nextPos(double*, double*, double*);
	bool isNotHeaderRead = true;
	bool isFileOpen = false;
	char* fileNamewPath;
	FULLFrame frame;
	LLHCoordinate LLHcoords;
	ECEFCoordinate ECEFcoords;
	enum CoordinateType {ECEF,LLH};
	CoordinateType coordType;
	CoordinateFrameHandler coordFHandler;
	std::ifstream trajFile;
};

