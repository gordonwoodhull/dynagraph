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

namespace Dynagraph {
namespace DynaDAG {

void Config::resetRankBox(Rank *rank) {
	Ranks::index r = Ranks::Xlator::CoordToRank(whole,rank->yBase);
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
}

void Config::resetBaselines() {
}

void Config::SetYs() {
  Ranks::iterator ri;
	for(ri = ranking.begin(); ri!=ranking.end(); ++ri)
		resetRankBox(*ri);

	resetBaselines();

	for(ri = ranking.begin(); ri!=ranking.end(); ++ri)
		for(NodeV::iterator ni = (*ri)->order.begin(); ni!=(*ri)->order.end(); ++ni) {
			DDNode &ddn = gd<DDNode>(*ni);
			double newY = (*ri)->yBase;
			ddn.cur.y = newY;
		}
}

} // namespace DynaDAG
} // namespace Dynagraph
