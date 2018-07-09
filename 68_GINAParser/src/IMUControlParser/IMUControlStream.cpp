
#include "IMUControlStream.hpp"

namespace GINAParser {
	IMUControlStream::IMUControlStream(){
		init();
	}

	IMUControlStream::IMUControlStream(const char* fn, std::ios::openmode mode)
		: FFTextStream(fn, mode){
		init();
	}

	IMUControlStream::~IMUControlStream(){
	}

	void IMUControlStream ::
		open(const char* fn, std::ios::openmode mode){
		FFTextStream::open(fn, mode);
		init();
	}

	void IMUControlStream ::
		init(){
		headerRead = false;
		header = IMUControlHeader();
	}

}