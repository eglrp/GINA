#pragma once

typedef struct LLHCoordinate_s{
	double latitude;
	double longitude;
	double height;
}LLHCoordinate;

typedef struct ECEFCoordinate_s{
	double x;
	double y;
	double z;
}ECEFCoordinate;

typedef struct GPSTime_s {
	unsigned int WN;
	unsigned int ToW;
}GPSTime;

typedef struct FullFrame_s {
	GPSTime GPSTime;
	LLHCoordinate LLHcoordinates;
	ECEFCoordinate ECEFcoordinates;
}FULLFrame;