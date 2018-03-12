#pragma once
#include "Data.h"
#include "PinaFile.h"
#include "Header.h"
class ConcreteDataProcess : public Data
{
public:

	ConcreteDataProcess();
	vector<string> type;

	vector<Data> data;
	void copy(PinaFile pina_file);
	void print();

	~ConcreteDataProcess();
};