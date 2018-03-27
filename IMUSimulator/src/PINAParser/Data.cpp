#include "stdafx.h"
#include "Data.h"
#include "Pinafile.h"
#include "Header.h"

void Data::load_position(double* new_position)
{
	int length = sizeof(position) / sizeof(double);
	for (int i = 0;i < length;++i) {
		position[i] = new_position[i];
	}
}

void Data::load_attitude(double* new_attitude)
{
	int length = sizeof(attitude) / sizeof(double);
	for (int i = 0;i < length;++i) {
		attitude[i] = new_attitude[i];
	}
}


Data::Data()
{
}

Data::Data(vector<double> one_row,vector<string> type)
{
	time[0]=one_row[0];
	time[1]=one_row[1];

	if (type == Header::IMU_type) {
		acceleration[0] = one_row[2];
		acceleration[1] = one_row[3];	
		acceleration[2] = one_row[4];
	
		angular_velocity[0] = one_row[5];
		angular_velocity[1] = one_row[6];
		angular_velocity[2] = one_row[7];
	}
	if (type == Header::trajectory_type) {
		position[0] = one_row[2];
		position[1] = one_row[3];
		position[2] = one_row[4];

		attitude[0] = one_row[5];
		attitude[1] = one_row[6];
		attitude[2] = one_row[7];
	}
}

Data::~Data()
{
}
string Data::start_of_data = "START OF DATA";