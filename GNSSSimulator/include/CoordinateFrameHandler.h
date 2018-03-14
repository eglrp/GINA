#pragma once

#include "CoordinateFrames.h"
#include "Position.hpp"


class CoordinateFrameHandler {

public:
	void convertToECEF(double*,double*,double*);
	void convertToLLH(double*, double*, double*);
private:
	gpstk::Position pos;
};
