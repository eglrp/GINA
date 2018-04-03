#pragma once
#include "Data.h"
#include "PinaFile.h"
#include "Header.h"
class ConcreteDataProcess : public Data // Header 
{
public:

	ConcreteDataProcess();
	vector<string> type;	// IMU_DATA, trajectory
	string position_format; // LLH,ECEF

	vector<Data> data;		
	
	double start_time[2];
	double end_time[2];

	double start_position[3];
	double start_velocity[3];
	double start_attitude[3];

	double epoch_time;

	void copy(PinaFile pina_file);
    void print_start_values(void);
	void print_data();

	~ConcreteDataProcess();
};