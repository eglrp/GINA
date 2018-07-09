#pragma once

#include "FFTextStream.hpp"
#include "IMUHeader.hpp"

namespace GINAParser {

	class IMUStream : public gpstk::FFTextStream
	{
	public:
		IMUStream();

		/** Constructor
		* Opens a file named \a fn using ios::openmode \a mode.
		*/
		IMUStream(const char* fn, std::ios::openmode mode = std::ios::in);

		/// Destructor
		virtual ~IMUStream();

		/// overrides open to reset the header
		virtual void open(const char* fn, std::ios::openmode mode);

		/// RINEX NAV header for this file.
		IMUHeader header;
		
		/// Flag showing whether or not the header has been read.
		bool headerRead = false;
		bool dataStartRead= false;
		bool dataStartWritten = false;

	private:
		void init();
	};

	//@}


}

