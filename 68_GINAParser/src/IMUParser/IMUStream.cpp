
#include "IMUStream.hpp"

namespace GINAParser {
	IMUStream::IMUStream(){
		init();
	}

	IMUStream::IMUStream(const char* fn, std::ios::openmode mode)
		: FFTextStream(fn, mode){
		init();
	}

	IMUStream::~IMUStream(){
	}

	void IMUStream ::
		open(const char* fn, std::ios::openmode mode){
		FFTextStream::open(fn, mode);
		init();
	}

	void IMUStream ::
		init(){
		headerRead = false;
		header = IMUHeader();
	}

}