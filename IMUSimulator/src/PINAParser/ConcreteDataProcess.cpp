#include "stdafx.h"
#include "ConcreteDataProcess.h"
#include "Data.h"

ConcreteDataProcess::ConcreteDataProcess(){
}

ConcreteDataProcess::~ConcreteDataProcess(){
}

void ConcreteDataProcess::copy(PinaFile pina_file){
	data = pina_file.data;
	type = pina_file.header.type;
	position_format = pina_file.header.position_format;

	start_time[0]= pina_file.start_time[0];
	start_time[1] = pina_file.start_time[1];

	end_time[0] = pina_file.end_time[0];
	end_time[1] = pina_file.end_time[1];

	position_format = pina_file.position_format;

	for (int i = 0;i< sizeof(double[3])/sizeof(double);++i) {
		start_position[i] = pina_file.header.start_position[i];
		start_velocity[i] = pina_file.header.start_velocity[i];
		start_attitude[i] = pina_file.header.start_attitude[i];
	}
	epoch_time = pina_file.header.epoch_time;
}

void ConcreteDataProcess::print_data()
{

	cout << "\nDATA:\n" << endl;
	for (vector<Data>::const_iterator one_data = data.begin(); one_data != data.end(); one_data = one_data + 1) {
		//cout << (*one_data).time[0] << " " << (*one_data).time[1] << " ";
		if (type == Header::IMU_type) {
			cout << (*one_data).acceleration[0] << " " << (*one_data).acceleration[1] << " " << (*one_data).acceleration[2] << " ";
			cout << (*one_data).angular_velocity[0] << " " << (*one_data).angular_velocity[1] << " " << (*one_data).angular_velocity[2] << endl;
		}
		if (type == Header::trajectory_type) {
			cout << (*one_data).position[0] << " " << (*one_data).position[1] << " " << (*one_data).position[2] << " ";
			cout << (*one_data).attitude[0] << " " << (*one_data).attitude[1] << " " << (*one_data).attitude[2] << endl;
		}
	}
}
void  ConcreteDataProcess::print_start_values(void)
{

	cout << "\nSTART VALUES:\n" << endl;
	cout << start_position[0] << " " << start_position[1] << " " << start_position[2] << endl;
	cout << start_velocity[0] << " " << start_velocity[1] << " " << start_velocity[2] << endl;
	cout << start_attitude[0] << " " << start_attitude[1] << " " << start_attitude[2] << endl;
}