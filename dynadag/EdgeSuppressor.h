/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/

#ifndef EdgeSuppressor_h
#define EdgeSuppressor_h

#include "FlexiRanks.h"

namespace Dynagraph {
namespace DynaDAG {

// (Layout supports NSRankerNode,NSRankerEdge,Suppression)
template<typename Layout>
struct EdgeSuppressor : LinkedChangeProcessor<Layout> {
	EdgeSuppressor(ChangingGraph<Layout> *world) : LinkedChangeProcessor<Layout>(world) {}

	typedef FlexiRankXlator<Layout> RankXlator;
	bool findEdgeSuppression(typename Layout::Edge *e,int thirrank) {
		typename Layout::Node *t = e->tail, *h = e->head;
		Suppression::Suppressionness suppression;
		int suppressRank(-17);
		if(gd<NodeGeom>(t).suppressed)
			if(gd<NodeGeom>(h).suppressed)
				suppression = Suppression::suppressed;
			else {
				suppression = Suppression::tailSuppressed;
				suppressRank = getEdgeDirection(e)==reversed
					? gd<NSRankerNode>(h).newBottomRank+thirrank
					: gd<NSRankerNode>(h).newTopRank-thirrank;
			}
		else if(gd<NodeGeom>(h).suppressed) {
			suppression = Suppression::headSuppressed;
			suppressRank = getEdgeDirection(e)==reversed
				? gd<NSRankerNode>(t).newTopRank-thirrank
				: gd<NSRankerNode>(t).newBottomRank+thirrank;
		}
		else suppression = Suppression::unsuppressed;
		bool ret = assign(gd<Suppression>(e).suppression,suppression);
		if(suppression==Suppression::headSuppressed || suppression==Suppression::tailSuppressed)
			ret |= assign(gd<Suppression>(e).suppressRank,suppressRank);
		return ret;
	}
	void Process() {
		int thirrank = RankXlator::HeightToDRank(&this->world_->whole_,gd<GraphGeom>(&this->world_->whole_).separation.y/3.);
		for(typename Layout::graphedge_iter ei = this->world_->current_.edges().begin(); ei!=this->world_->current_.edges().end(); ++ei) {
			if(findEdgeSuppression(*ei,thirrank))
				ModifyEdge(this->world_->Q_,*ei,DG_UPD_SUPPRESSION);
			if(gd<Suppression>(*ei).suppression==Suppression::tailSuppressed || gd<Suppression>(*ei).suppression==Suppression::headSuppressed)
				gd<ExtraRanks>(&this->world_->whole_).extraRanks.push_back(gd<Suppression>(*ei).suppressRank);
		}
		this->NextProcess();
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // EdgeSuppressor_h
