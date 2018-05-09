#include "stdafx.h"
#include "Header.h"
#include "Pinafile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "ConcreteDataProcess.h"
using namespace std;

Header::Header()
{
}

Header::Header(string file_name){
	//string double_type_header_data[6];
	double_type_header_data[0] = "START TIME";
	double_type_header_data[1] = "END TIME";
	double_type_header_data[2] = "START POSITION";
	double_type_header_data[3] = "START VELOCITY";
	double_type_header_data[4] = "START ATTITUDE";
	double_type_header_data[5] = "EPOCH TIME";

	//string string_type_header_data[5];
	string_type_header_data[0] = "TYPE";

	string_type_header_data[1] = "CREATOR";
	string_type_header_data[2] = "TIME OF CREATION";
	string_type_header_data[3] = "TIME SYSTEM";
	string_type_header_data[4] = "POSITION FORMAT";

	load_file(file_name, string_type_header_data,double_type_header_data);
}

Header::~Header()
{
}

void Header::hand_out_double_data(string keyword, vector<double> data) {
	if (keyword == "START TIME") { 
		//cout <<" $start time$ " << data[0] <<" "<<  data[1]<< endl;
		std::copy(data.begin(), data.end(), start_time);
	}
	if (keyword == "END TIME") {
		copy(data.begin(), data.end(), end_time);
		//cout << " $end time$ " << data[0] << " " << data[1] << endl;
	}
	if (keyword == "START POSITION") { 
		copy(data.begin(), data.end(), start_position);
		//cout << " $start position$ " << data[0] << " " << data[1] << " " << data[2] << endl;
	}
	if (keyword == "START VELOCITY") { 
		copy(data.begin(), data.end(), start_velocity);
		//cout << " $start velocity$ " << data[0] << " " << data[1] << " " << data[2] << endl;
	}
	if (keyword == "START ATTITUDE") { 
		copy(data.begin(), data.end(), start_attitude);
		//cout << " $start attitude$ " << data[0] << " " << data[1] << " " << data[2] << endl;
	}
	if (keyword == "EPOCH TIME") { 
		//epoch_time = data[0];
		//cout << " $epoch$ " << data[0]<< endl;
		epoch_time = data[0];
		//copy(data.begin(), data.end(), epoch_time);
	}
}
void Header::hand_out_string_data(string keyword, vector<string>data) {
	if (keyword == "CREATOR") { 
		//copy(data.begin(), data.end(), creator);
		//cout << " $CREATOR$ " << data[0] << endl;
		creator = data;
	}
	if (keyword == "TIME OF CREATION") { 
		//copy(data[1].begin(), data[1].end(), time_of_creation);
		//cout << " $TIME OF CREATION$ " << data[0] <<endl;
		time_of_creation = data[0];
	}
	if (keyword == "TIME SYSTEM") { 
		//copy(data[2].begin(), data[2].end(), time_system);
		//cout << " $TIME SYSTEM$ " << data[0]<< endl;
		time_system = data[0];
	}
	if (keyword == "POSITION FORMAT") { 
		//copy(data[3].begin(), data[3].end(), position_format);
		//cout << " $POSITION FORMAT$ " << data[0]<< endl;
		position_format  = data[0];
	}
	if (keyword == "TYPE") {
		type = data;
		//cout << " $TYPE$ " << data[0] << endl;
		
	}
}
vector<string> Header::split_to_string(string line) {
	const char *char_line = line.c_str();
	vector<string> split_line;
	int pos = line.length();
	//Read and split the string of numbers reverse.
	while (pos >= -1) {
		string string_of_number;
		while (char_line[pos] != ' ' && char_line[pos] != '\t' && char_line[pos] != '\n' && char_line[pos] != '\0' && pos >= 0) {
			string_of_number += char_line[pos];
			--pos;
		}
		if (string_of_number.length() != 0) {
			split_line.push_back(string_of_number);
		}
		--pos;
	}
	//Now the numbers are in the split_line vector, but they are mirrored.
	//Mirroring
	for (int i = 0;i < split_line.size();++i) {
		int pos = split_line[i].length();
		string mirrored_number = split_line[i];
		reverse(mirrored_number.begin(), mirrored_number.end());
		split_line[i] = mirrored_number;
	}
	// 3rd,2nd,1st -> 1st,2nd,3rd
	if (split_line.begin() != split_line.end()) {
		reverse(split_line.begin(), split_line.end());
	}
	//Result the data, in a vector<double>.
	return split_line;
}
vector<double> Header::split_to_double(string line) {
	const char *char_line = line.c_str();
	vector<string> split_line;
	int pos = line.length();
	//Read and split the string of numbers reverse.
	while (pos >= -1) {
		string string_of_number;
		while (char_line[pos] != ' ' && char_line[pos] != '\t' && char_line[pos] != '\n' && char_line[pos] != '\0' && pos >= 0) {
			string_of_number += char_line[pos];
			--pos;
		}
		if (string_of_number.length() != 0) {
			split_line.push_back(string_of_number);
		}
		--pos;
	}
	//Now the numbers are in the split_line vector, but they are mirrored.     
	//Mirroring
	for (int i = 0;i < split_line.size();++i) {
		int pos = split_line[i].length();
		string mirrored_number = split_line[i];
		reverse(mirrored_number.begin(), mirrored_number.end());
		split_line[i] = mirrored_number;
	}
	//String to double conversion
	vector<double> data;
	for (int i = 0;i<split_line.size();++i) {
		data.push_back(stod(split_line[i]));
	}

	// 3rd,2nd,1st -> 1st,2nd,3rd
	if (data.begin() != data.end()) {
		reverse(data.begin(), data.end());
	}
	//Result the data, in a vector<double>.
	return data;
}
string Header::turncate_comment(string line) {
	string turncated_string;
	const char *char_line = line.c_str();
	int pos = 0;
	while (char_line[pos] != '\0') {
		if (char_line[pos] == '#') {
			break;
		}
		turncated_string += char_line[pos];
		++pos;
	}
	return turncated_string;
}
/*Find an expression (keyword) within a line and if it is found, return the
rest of string after the word.*/
string Header::find_data(string line, string expression) {
	size_t pos = 0;
	char space = ' ';
	line = turncate_comment(line);
	if (line.find(expression) != std::string::npos) {
		pos = line.find((string)expression);
		pos = pos + expression.length() + sizeof(space);
		return turncate_comment(line.substr(pos)) + '\n';
	}
	else {
		return std::string();
	}
}
void Header::load_file(string file_name, string string_type_header_data[], string double_type_header_data[]) {

	ifstream pina_file;
	//pina_file.open(file_name);
	if (!pina_file) {
		cout << "Can not open the file! Please check the filename or the place of file!" << endl;
	}
	string line_of_pina;

	vector<double> data_d;
	vector<string> data_s;
	//Handout double format data.
	int max = 6;//sizeof(double_type_header_data)/sizeof(double_type_header_data[0]);
	for (int i = 0; i < max ; ++i){
		pina_file.open(file_name);
		string keyword_d = double_type_header_data[i];
		sizeof(double_type_header_data);
		string keyword_s;
		if (i >=5){
			//keyword_s = "EGY OLYAN STRING AMI TUTIRA NINCS BENNE....";
			keyword_s = std::string::npos;
		}else { 
				keyword_s = string_type_header_data[i];
		}

		bool header = true;
		bool find_string_type_data = false;
		bool find_double_type_data = false;

		data_d.clear();
		data_s.clear();
		int line_counter = 0;
		while (getline(pina_file, line_of_pina) && header == true) {
			//cout << line_of_pina << endl;
			if (line_of_pina.find("END OF HEADER") != std::string::npos) {
				header = false;
				pina_file.close();
			}
			string found_string = find_data(line_of_pina, keyword_s);
			if (found_string.length() != 0) {
				find_string_type_data = true;
				data_s = split_to_string(found_string);
			}

			string found_double_stirng = find_data(line_of_pina, keyword_d);
			if (found_double_stirng.length() != 0) {
				find_double_type_data = true;
				data_d = split_to_double(found_double_stirng);
			}

			if (find_string_type_data == true) {
				hand_out_string_data(keyword_s, data_s);
			}

			if (find_double_type_data == true) {
				hand_out_double_data(keyword_d, data_d);
			}

			find_string_type_data = false;
			find_double_type_data = false;

			line_counter++;
			if (line_counter == 2 && type.size() == 0) {
				puts("Missing type of document at section of header.");
				abort();
			}
		}
	}
}

void Header::print_info(){
	cout << "\n" << endl;
	cout << "TYPE ";
	for (vector<string>::const_iterator i = type.begin(); i != type.end(); ++i) cout << *i << ' ';
	cout << endl;
	cout << "CREATOR ";
	for (vector<string>::const_iterator i = creator.begin(); i != creator.end(); ++i) cout << *i << ' ';
	cout << endl;
	cout << "TIME OF CREATION " << time_of_creation << endl;
	cout << "TIME SYSTEM " << time_system << endl;
	cout << "START TIME " << start_time[0] <<" "<< start_time[1] << endl;
	cout << "END TIME " << end_time[0] << " " << end_time[1] << endl;
	cout << "POSITION FORMAT " << position_format << endl;
	cout << "START POSITION " << start_position[0] << " " << start_position[1] << " " << start_position[2] << endl;
	cout << "START VELOCITY " << start_velocity[0] << " " << start_velocity[1] << " " << start_velocity[2] << endl;
	cout << "START ATTITUDE " << start_attitude[0] << " " << start_attitude[1] << " " << start_attitude[2] << endl;
	cout << "EPOCH TIME " << epoch_time << endl;
}

void Header::clear()
{

	type.clear();

	creator.clear();
    time_of_creation.clear();

    time_system.clear();
	start_time[0] = NULL;
	start_time[1] = NULL;

	end_time[0] = NULL;
	end_time[1] = NULL;

	position_format.clear();

	start_position[0] = NULL;
	start_position[1] = NULL;
	start_position[2] = NULL;

	start_velocity[0] = NULL;
	start_velocity[1] = NULL;
	start_velocity[2] = NULL;

	
	start_attitude[0] = NULL;
	start_attitude[1] = NULL;
	start_attitude[2] = NULL;

	epoch_time = NULL;
}

string Header::end_of_header = "END OF HEADER";
vector<string> Header::IMU_type = { "PINA","IMU","BODY","DATA" };
vector < string> Header::trajectory_type = { "PINA","TRAJECTORY" };
