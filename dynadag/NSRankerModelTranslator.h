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


#ifndef NSRankerModelTranslator_h
#define NSRankerModelTranslator_h

#include "ChangeProcessor.h"
#include "NSRankerModel.h"
#include "common/QueueTransitions.h"

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout>
struct LayoutToNSRankerModelTranslator : ChangeTranslator<Layout,NSRankerModel> {
	GoingQueueTransition<Layout,NSRankerModel> transition_;
	void Process(ChangeQueue<Layout> &Q) {
		ChangeQueue<NSRankerModel> &Q2 = transition_.NextQ();

		// insert these in Q2 (which doesn't return) and mod them in Q1
        for(typename Layout::graphedge_iter ei = source_->Q.delE.edges().begin(); ei!=source_->Q.delE.edges().end();++ei)
            if(typename Layout::Edge *e2 = source_->current_.find_edge((*ei)->head,(*ei)->tail))
                if(assign(gd<NSRankerEdge>(e2).secondOfTwo,false))
                    extraI.insert(e2);

        for(typename Layout::node_iter ni = changeQ.delN.nodes().begin(); ni!=changeQ.delN.nodes().end();++ni)
            removeLayoutNodeConstraints(*ni);

		transition_.EndLastQ(Q);
		LinkedChangeProcessor<Graph2>::NextProcess(Q2);
	}
};
template<typename Layout>
struct NSRankerModelToLayoutTranslator : ChangeTranslator<NSRankerModel,Layout> {
	ReturningQueueTransition<NSRankerModel,Layout> transition_;
	FlexiRankXlator rankXlate_;
	NSRankerModelToLayoutTranslator(FlexiRankXlator rankXlate) : rankXlate_(rankXlate) {}
	void Process(ChangeQueue<NSRankerModel> &Q) {
		ChangeQueue<Layout> &Q2 = transition_.NextQ();

		// Since NS is not yet incremental, Q is blank.
		// Just read whole graph and find changes.
		int anchorRank = DDNS::NSd(gd<NSRankerModelGraph>(Q.current_).cg.anchor).rank;
		for(NSRankerModel::node_iter ni = Q.current->nodes().begin(); ni!=Q.current->nodes().end(); ++ni) {
			NSRankerModel::Node *mn = *ni;
			typename Layout::Node *n = Q2.current->fetch_node(gd<Name>(mn),false);
			assert(n);
			if(Q2.delN.find(n))
				continue;
			int newTopRank = DDNS::NSd(gd<NSRankerModelNode>(mn).topC.n).rank - anchorRank,
				newBottomRank = DDNS::NSd(gd<NSRankerModelNode>(mn).bottomC.n).rank - anchorRank;
			assert(!rankXlate_.Below(newTopRank,newBottomRank));
			if(newTopRank != gd<NSRankerNode>(n).oldTopRank || newBottomRank != gd<NSRankerNode>(n).oldBottomRank) {
				gd<NSRankerNode>(n).newTopRank = newTopRank;
				gd<NSRankerNode>(n).newBottomRank = newBottomRank;
				ModifyNode(Q2,n,DG_UPD_MOVE);
				typename Layout::Node *cn = Q2.current->find(n);
				for(typename Layout::nodeedge_iter ei=cn->alledges().begin(); ei!=cn->alledges().end(); ++ei)
					ModifyEdge(Q2,*ei,DG_UPD_MOVE);
			}
		}
		transition_.EndLastQ(Q);
		LinkedChangeProcessor<Layout>::NextProcess(Q2);
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerModelTranslator_h
