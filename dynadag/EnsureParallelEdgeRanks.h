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

#ifndef EnsureParallelEdgeRanks_h
#define EnsureParallelEdgeRanks_h

#include "FlexiRanks.h"

namespace Dynagraph {
namespace DynaDAG {

// (Layout supports NSRankerNode,NSRankerEdge,Suppression)
template<typename Layout>
struct EnsureParallelEdgeRanks : ChangeProcessor<Layout> {
	EnsureParallelEdgeRanks(ChangingGraph<Layout> *world) : ChangeProcessor<Layout>(world) {}

	typedef FlexiRankXlator<Layout> RankXlator;
	void Process(ChangeProcessing *next) {
		for(typename Layout::graphedge_iter ei = this->world_->current_.edges().begin(); ei!=this->world_->current_.edges().end(); ++ei) {
			typename Layout::Node *t = (*ei)->tail, *h = (*ei)->head;
			typename Layout::headedge_iter_pair hip = this->world_->current_.find_edge(t,h);
			if(++hip.first!=hip.second || this->world_->current_.has_edge(h,t)) { // parallel edge or 2-cycle
				switch(getEdgeDirection(*ei)) {
					case reversed:
						std::swap(t,h);
						break;
					case flat:
						continue;
					case forward: ;
				}
				int tb = gd<NSRankerNode>(t).newBottomRank,
					ht = gd<NSRankerNode>(h).newTopRank,
					mid = (tb+ht)/2;
				// could check to see if there's already a rank there...
				gd<ExtraRanks>(&this->world_->current_).extraRanks.insert(mid);
			}
		}
		this->NextProcess();
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // EnsureParallelEdgeRanks_h
