/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/


#include "LayoutAttrs.h"
#include "Transform.h"
#include <sstream>
#include <iomanip>
using namespace std;

namespace Dynagraph {

// these write dynagraph Geom changes to the StrAttrs and StrAttrChanges attributes
DString g_NLPNames[5] = {"center","right","top","left","bottom"};

const double ln10 = 2.30258509299404568401799145468436;
void initStream(std::ostringstream &o,const Coord &resolution) {
	double lbase10 = log(resolution.x)/ln10;
	int precX = int(-floor(lbase10));
	o.flags(o.flags()|std::ios::fixed);
	o.width(0);
	o.precision(precX);
}


} // namespace Dynagraph
