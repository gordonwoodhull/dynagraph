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
				if(getEdgeDirection(e)==reversed) {
					suppressRank = gd<NSRankerNode>(h).newBottomRank+thirrank;
					dgassert(suppressRank < gd<NSRankerNode>(t).newTopRank);
				}
				else {
					suppressRank = gd<NSRankerNode>(h).newTopRank-thirrank;
					dgassert(suppressRank > gd<NSRankerNode>(t).newBottomRank);
				}
			}
		else if(gd<NodeGeom>(h).suppressed) {
			suppression = Suppression::headSuppressed;
			if(getEdgeDirection(e)==reversed) {
				suppressRank = gd<NSRankerNode>(t).newTopRank-thirrank;
				dgassert(suppressRank > gd<NSRankerNode>(h).newBottomRank);
			}
			else {
				suppressRank = gd<NSRankerNode>(t).newBottomRank+thirrank;
				dgassert(suppressRank < gd<NSRankerNode>(h).newTopRank);
			}
		}
		else suppression = Suppression::unsuppressed;
		/*
		if(suppression)
			std::cerr << "Suppression " << suppression << " suppressRank " << suppressRank << " direction " << getEdgeDirection(e)  << 
				" tail top " << gd<NSRankerNode>(t).newTopRank << " tail bottom " << gd<NSRankerNode>(t).newBottomRank <<
				" head top " << gd<NSRankerNode>(h).newTopRank << " head bottom " << gd<NSRankerNode>(h).newBottomRank << std::endl;
				*/
		bool ret = assign(gd<Suppression>(e).suppression,suppression);
		if(suppression==Suppression::headSuppressed || suppression==Suppression::tailSuppressed)
			ret |= assign(gd<Suppression>(e).suppressRank,suppressRank);
		return ret;
	}
	void Process() {
		int thirrank = RankXlator::SeparationFrac(&this->world_->whole_,3);
		for(typename Layout::graphedge_iter ei = this->world_->current_.edges().begin(); ei!=this->world_->current_.edges().end(); ++ei) {
			if(findEdgeSuppression(*ei,thirrank))
				ModifyEdge(this->world_->Q_,*ei,DG_UPD_SUPPRESSION);
			if(gd<Suppression>(*ei).suppression==Suppression::tailSuppressed || gd<Suppression>(*ei).suppression==Suppression::headSuppressed)
				gd<ExtraRanks>(&this->world_->whole_).extraRanks.insert(gd<Suppression>(*ei).suppressRank);
		}
		this->NextProcess();
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // EdgeSuppressor_h
