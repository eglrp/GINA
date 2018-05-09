#include "stdafx.h"
#include "CoordinateFrameHandler.h"



void CoordinateFrameHandler::convertToECEF(double * coor1, double * coor2, double * coor3)
{
	pos.setGeodetic(*coor1, *coor2, *coor3);
	pos.transformTo(gpstk::Position::Cartesian);
	*coor1 = pos.X();
	*coor2 = pos.Y();
	*coor3 = pos.Z();
}

void CoordinateFrameHandler::convertToLLH(double * coor1, double * coor2, double * coor3)
{
	pos.setECEF(*coor1, *coor2, *coor3);
	pos.transformTo(gpstk::Position::Geodetic);
	*coor1 = pos.getGeodeticLatitude();
	*coor2 = pos.longitude();
	*coor3 = pos.height();
}

