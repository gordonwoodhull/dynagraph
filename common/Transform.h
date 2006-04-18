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


#ifndef Transform_h
#define Transform_h

#include "Geometry.h"

namespace Dynagraph {

// dynagraph uses dimensionless up-positive coords, but not everyone else does!
// so when it reads or writes .dot files or talks over a pipe, it uses one of these:
// multiplies by relevant ratios on read, divides on write
// wouldn't it make sense to give this responsibility for default sizes too?
class Transform {
	Coord nodeRatio, // for node size
		coordRatio; // for node position, edge coords, and bounding box
public:
	Coord ll; // lower left corner
	Transform(Coord nr,Coord cr) : nodeRatio(nr),coordRatio(cr),ll(Coord(0,0)) {}

	Coord inSize(Coord c) {
		return Coord(c.x*nodeRatio.x,c.y*nodeRatio.y);
	}
	Coord outSize(Coord c) {
		return Coord(c.x/nodeRatio.x,c.y/nodeRatio.y);
	}
	Coord in(Coord c) {
		return Coord((c+ll).x*coordRatio.x,(c+ll).y*coordRatio.y);
	}
	Coord out(Coord c) {
		return Coord(c.x/coordRatio.x-ll.x,c.y/coordRatio.y-ll.y);
	}
	Rect in(Rect r) {
		Coord ul = in(Coord(r.l,r.t)),
			lr = in(Coord(r.r,r.b));
		return Rect(ul.x,ul.y,lr.x,lr.y);
	}
	Rect out(Rect r) {
		Coord ul = out(Coord(r.l,r.t)),
			lr = out(Coord(r.r,r.b));
		return Rect(ul.x,ul.y,lr.x,lr.y);
	}
};
// dot transform: node size in inches, coords in down-positive points
// (dynagraph will do calcs in up-positive points)
extern Transform g_dotRatios;

bool transformShape(Transform *trans,Line &shape);

} // namespace Dynagraph

#endif // Transform_h
