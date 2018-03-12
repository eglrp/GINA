/*Description*/
#pragma once
#include "stdafx.h"
#include "Header.h"
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
class Header
{	
public:
	Header();
	Header(string line);

	~Header();

	vector<string> type;

	vector<string> creator;
	string time_of_creation;

	string time_system;
	double start_time[2];
	double end_time[2];

	string position_format;

	double start_position[3];
	double start_velocity[3];
	double start_attitude[3];

	double epoch_time;

	string double_type_header_data[6];
	string string_type_header_data[5];
	//Functions to the constructor
	void hand_out_double_data(string keyword, vector<double> data_d);
	void hand_out_string_data(string keyword, vector<string> data_s);
	vector<double> split_to_double(string line);
	vector<string> split_to_string(string line);
	string turncate_comment(string line);
	string find_data(string line, string expression);
	void load_file(string file_name, string  double_type_header_data[], string string_type_header_data[]);
	//Other functions;
	void print_info();
	void clear();

	static string end_of_header;
	static vector<string> IMU_type;
	static vector<string> trajectory_type;

private:
};


