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

#ifndef Rank_h
#define Rank_h

namespace Dynagraph {
namespace DynaDAG {

struct Rank {
	NodeV order;
	double yBase,	/* absolute */
		deltaAbove, deltaBelow, spaceBelow;
	Rank(double sep) : yBase(-17),deltaAbove(sep/20),
	  deltaBelow(sep/20), spaceBelow(sep) {}
	Rank(Rank &o) : order(o.order),yBase(o.yBase),
	  deltaAbove(o.deltaAbove),deltaBelow(o.deltaBelow) {}
	double yBelow(double fract) {
		return yBase;
	}
	double yAbove(double fract) {
		return yBase + 2*deltaAbove;
	}
	double Height() {
		return deltaAbove+deltaBelow+spaceBelow;
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Rank_h
