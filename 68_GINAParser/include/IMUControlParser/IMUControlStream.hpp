#pragma once

#include "FFTextStream.hpp"
#include "IMUControlHeader.hpp"

namespace GINASimulator {

	class IMUControlStream : public gpstk::FFTextStream
	{
	public:
		IMUControlStream();

		/** Constructor
		* Opens a file named \a fn using ios::openmode \a mode.
		*/
		IMUControlStream(const char* fn, std::ios::openmode mode = std::ios::in);

		/// Destructor
		virtual ~IMUControlStream();

		/// overrides open to reset the header
		virtual void open(const char* fn, std::ios::openmode mode);

		/// RINEX NAV header for this file.
		IMUControlHeader header;
		
		/// Flag showing whether or not the header has been read.
		bool headerRead = false;
		bool dataStartRead= false;
		bool dataStartWritten = false;

	private:
		void init();
	};

	//@}


}

