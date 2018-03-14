#pragma once
#include "Header.h"

class Data : public Header
{
public:
	double time[2];

	double acceleration[3];
	double angular_velocity[3];

	double position[3];
	double attitude[3];

	static string start_of_data;

	Data();
	Data(vector<double> one_row, vector<string> type);
	~Data();
};

