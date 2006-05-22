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

#ifndef NSRankerAttrs_h
#define NSRankerAttrs_h

namespace Dynagraph {
namespace DynaDAG {

struct NSRankerNode {
	int newTopRank,	// destination rank assignment
		newBottomRank,
		oldTopRank,	// previous rank assignment
		oldBottomRank;
	NSRankerNode() : newTopRank(0),newBottomRank(0),oldTopRank(0),oldBottomRank(0) {}
};
struct NSRankerEdge {
	// the second edge of 2-cycle should be ignored, mostly
	bool secondOfTwo;
	NSRankerEdge() : secondOfTwo(false) {}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerAttrs_h
