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


#include "DynaDAG.h"

namespace DynaDAG {

void Config::resetRankBox(Rank *rank) {
#ifdef FLEXIRANKS
	Ranks::index r = ranking.y2r(rank->yBase);
	Ranks::iterator i = ranking.GetIter(r);
	rank->deltaAbove = 0;
	if(i!=ranking.begin())
		rank->deltaAbove = ((*--i)->yBase-rank->yBase)/2.0;
	i = ranking.GetIter(r);
	rank->deltaBelow = 0;
	if(++i!=ranking.end())
		rank->deltaBelow = (rank->yBase-(*i)->yBase)/2.0;
	if(!rank->deltaAbove)
		rank->deltaAbove = rank->deltaBelow;
	if(!rank->deltaBelow)
		rank->deltaBelow = rank->deltaAbove;
	rank->spaceBelow = 0;
	//rank->deltaBelow -= rank->spaceBelow = rank->deltaBelow/10.0;
#else
	double maxTop = gd<GraphGeom>(config.client).separation.y / 20.0,
		maxBottom = gd<GraphGeom>(client).separation.y / 20.0;
	for(NodeV::iterator ni = rank->order.begin(); ni!=rank->order.end(); ++ni) {
		if(DDd(*ni).amEdgePart())
			continue;
		double nt = TopExtent(*ni);
		if(maxTop < nt)
			maxTop = nt;
		double nb = BottomExtent(*ni);
		if(maxBottom > nb)
			maxBottom = nb;
	}

	rank->deltaAbove = maxTop;
	rank->deltaBelow = maxBottom;
	rank->spaceBelow = gd<GraphGeom>(client).separation.y;
#endif
}

void Config::resetBaselines() {
#ifndef FLEXIRANKS
	if(ranking.empty())
		return;
	if(prevLow == INT_MAX)
		prevLow = ranking.Low();
	Rank *base = ranking.GetRank(prevLow);
	Ranks::iterator start = ranking.GetIter(prevLow);

	/* work upward from prevLow rank */
	Rank *prev = base;
	// (Note reverse_iterator::operator* returns *(fwdIt-1))
	for(Ranks::reverse_iterator rri(start); rri!=ranking.rend(); ++rri) {
		Rank *rank = *rri;
#ifndef DOWN_GREATER
		rank->yBase = prev->yAbove(1.0) + rank->deltaBelow;
#else
		rank->yBase = prev->yAbove(1.0) - rank->deltaBelow;
#endif
		prev = rank;
	}

	prev = base;
	for(Ranks::iterator ri = start+1; ri!=ranking.end(); ++ri) {
		Rank *rank = *ri;
#ifndef DOWN_GREATER
		rank->yBase = prev->yBelow(1.0) - rank->deltaAbove;
#else
		rank->yBase = prev->yBelow(1.0) + rank->deltaAbove;
#endif
		prev = rank;
	}
	prevLow = ranking.Low();
#endif
}

void Config::SetYs() {
  Ranks::iterator ri;
	for(ri = ranking.begin(); ri!=ranking.end(); ++ri)
		resetRankBox(*ri);

	resetBaselines();

	for(ri = ranking.begin(); ri!=ranking.end(); ++ri)
		for(NodeV::iterator ni = (*ri)->order.begin(); ni!=(*ri)->order.end(); ++ni) {
			DDNode &ddn = DDd(*ni);
			double newY = (*ri)->yBase;
			ddn.cur.y = newY;
		}
}

} // namespace DynaDAG
