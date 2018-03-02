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
}

void ConcreteDataProcess::print()
{
	cout << "\n" << endl;
	for (vector<Data>::const_iterator one_data = data.begin(); one_data != data.end(); one_data = one_data + 1) {
		cout << (*one_data).time[0] << " " << (*one_data).time[1] << " ";
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


