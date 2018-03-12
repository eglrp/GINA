#include "stdafx.h"
#include "TrajectoryStream.hpp"

namespace gnsssimulator {
	TrajectoryStream::TrajectoryStream(){
		init();
	}

	TrajectoryStream::TrajectoryStream(const char* fn, std::ios::openmode mode)
		: FFTextStream(fn, mode){
		init();
	}

	TrajectoryStream::~TrajectoryStream(){
	}

	void TrajectoryStream ::
		open(const char* fn, std::ios::openmode mode){
		FFTextStream::open(fn, mode);
		init();
	}

	void TrajectoryStream ::
		init(){
		headerRead = false;
		header = TrajectoryHeader();
	}

}