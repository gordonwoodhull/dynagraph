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

#include "Constraints.h"

namespace Dynagraph {
namespace DynaDAG {

struct NSRankerNode {
	NodeConstraints topC,bottomC;
	bool hit; // for rank dfs
	int newTopRank,	// destination rank assignment
		newBottomRank,
		oldTopRank,	// previous rank assignment
		oldBottomRank;
	bool rankFixed; // whether nailed in Y

	NSRankerNode() : hit(false),newTopRank(0),newBottomRank(0),oldTopRank(0),oldBottomRank(0),rankFixed(false) {}
	// do not copy constraints or hit-flag
	NSRankerNode(const NSRankerNode &other) :
		hit(false),
		newTopRank(other.newTopRank),newBottomRank(other.newBottomRank),oldTopRank(other.oldTopRank),oldBottomRank(other.oldBottomRank),
		rankFixed(other.rankFixed) {}
	NSRankerNode &operator=(NSRankerNode &other) {
		hit = false;
		newTopRank = other.newTopRank;
		newBottomRank = other.newBottomRank;
		oldTopRank = other.oldTopRank;
		oldBottomRank = other.oldBottomRank;
		rankFixed = other.rankFixed;
		return *this;
	}
};

struct NSRankerEdge {
	DDCGraph::Node *weak;
	DDCGraph::Edge *strong;
	// the second edge of 2-cycle should be ignored, mostly
	bool secondOfTwo;
	
	NSRankerEdge() : weak(0),strong(0),secondOfTwo(false) {}
	// do not copy constraints
	NSRankerEdge(const NSRankerEdge &other) : weak(0),strong(0),
		secondOfTwo(other.secondOfTwo) {}
	NSRankerEdge &operator=(NSRankerEdge &other) {
		secondOfTwo = other.secondOfTwo;
		return *this;
	}
};


} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerAttrs_h
