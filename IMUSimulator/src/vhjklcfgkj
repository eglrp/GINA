#include "stdafx.h"
#include "PinaFile.h"




PinaFile::PinaFile()
{
	header = Header();
}

PinaFile::PinaFile(string file_name)
{
	header = Header(file_name);
	load_data(file_name);
}

void PinaFile::open(string file_name)
{
	header = Header(file_name);
	load_data(file_name);
}

void PinaFile::load_data(string file_name){
	ifstream pina_file;
	pina_file.open(file_name);

	if (!pina_file) {
		cout << "Can not open the file! Please check the filename or the place of file!" << endl;
	}
	string data_of_pina;
	bool data_part = false;
	while (getline(pina_file, data_of_pina)) {
		if (data_part == true) {
			data.push_back(Data(split_to_double(data_of_pina), header.type));
		}
		
		if (data_of_pina == start_of_data) {
			data_part = true;
		}
	}
	//reverse(data.begin(),data.end());
}

void PinaFile::print_data() {
	cout << "\n" << endl;
	for (vector<Data>::const_iterator one_data = data.begin(); one_data != data.end(); one_data= one_data +1) {
		cout << (*one_data).time[0] << " " << (*one_data).time[1]<<" ";
		if (header.type == IMU_type) {
			cout << (*one_data).acceleration[0] << " " << (*one_data).acceleration[1] << " " << (*one_data).acceleration[2] << " ";
			cout << (*one_data).angular_velocity[0] << " " << (*one_data).angular_velocity[1] << " " << (*one_data).angular_velocity[2] << endl;
		}
		if (header.type == trajectory_type) {
			cout << (*one_data).position[0] << " " << (*one_data).position[1] << " " << (*one_data).position[2] << " ";
			cout << (*one_data).attitude[0] << " " << (*one_data).attitude[1] << " " << (*one_data).attitude[2] << endl;
		}
	}
}

void PinaFile::clear(){
	header.clear();
	data.clear(); 
}

PinaFile::~PinaFile()
{
}
