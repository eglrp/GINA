#pragma once
#include "Data.h"
#include <vector>

class IDataProcess
{
public:
	vector<Data>* data;
	virtual IDataProcess* clone() const = 0;
	IDataProcess();
	~IDataProcess();
};

