#pragma once
#include "Header.h"
#include "Data.h"

class PinaFile : public Data
{
public:
	PinaFile();
	PinaFile(string file_name);
	Header header;
	vector<Data> data;
	
	void open(string file_name);
	void load_data(string file_name);
	void print_data();
	
	void clear();

	~PinaFile();

private:
};

