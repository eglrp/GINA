#pragma once

#include "FFData.hpp"

namespace gnsssimulator {
		/// @ingroup FileHandling
		//@{

		/**
		* This class serves no purpose other than to make readable diagrams.
		*/
	class TrajectoryBase : public gpstk::FFData
	{
	protected:
		/// Does the actual reading from the stream into this FFData object.
		virtual void reallyGetRecord(gpstk::FFStream& s)
			throw(std::exception, gpstk::StringUtils::StringException,
				gpstk::FFStreamError) = 0;

		/// Does the actual writing from the stream into this FFData object.
		virtual void reallyPutRecord(gpstk::FFStream& s) const
			throw(std::exception, gpstk::StringUtils::StringException,
				gpstk::FFStreamError) = 0;

	public:
		TrajectoryBase() {}
		/// in deference to our coding standards, this is declared virtual
		virtual ~TrajectoryBase() {}
	};
		//@}
}