#pragma once

#include "FFTextStream.hpp"
#include "TrajectoryHeader.hpp"

namespace gnsssimulator {

	class TrajectoryStream : public gpstk::FFTextStream
	{
	public:
		TrajectoryStream();

		/** Constructor
		* Opens a file named \a fn using ios::openmode \a mode.
		*/
		TrajectoryStream(const char* fn, std::ios::openmode mode = std::ios::in);

		/// Destructor
		virtual ~TrajectoryStream();

		/// overrides open to reset the header
		virtual void open(const char* fn, std::ios::openmode mode);

		/// RINEX NAV header for this file.
		TrajectoryHeader header;

		/// Flag showing whether or not the header has been read.
		bool headerRead;

	private:
		void init();
	};

	//@}


}

