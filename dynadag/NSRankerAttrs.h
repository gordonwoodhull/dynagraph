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

#define NSRANKER_IS_A_BIG_BLOB
#ifdef NSRANKER_IS_A_BIG_BLOB
#include "Constraints.h"
#endif

namespace Dynagraph {
namespace DynaDAG {

struct NSRankerNode {
	int newTopRank,	// destination rank assignment
		newBottomRank,
		oldTopRank,	// previous rank assignment
		oldBottomRank;
#ifdef NSRANKER_IS_A_BIG_BLOB
	NodeConstraints topC,bottomC;
	bool hit, // for rank dfs
		rankFixed; // whether nailed in Y
#endif
	NSRankerNode() : newTopRank(0),newBottomRank(0),oldTopRank(0),oldBottomRank(0) 
#ifdef NSRANKER_IS_A_BIG_BLOB
		,hit(false),rankFixed(false)
#endif
	{}
};
struct NSRankerEdge {
	// the second edge of 2-cycle should be ignored, mostly
	bool secondOfTwo;
#ifdef NSRANKER_IS_A_BIG_BLOB
	DDCGraph::Node *weak;
	DDCGraph::Edge *strong;
#endif
	NSRankerEdge() : secondOfTwo(false)
#ifdef NSRANKER_IS_A_BIG_BLOB
		,weak(0),strong(0)
#endif
	{}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerAttrs_h
