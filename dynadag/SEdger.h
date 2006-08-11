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


#ifndef SEdger_h
#define SEdger_h

#include "common/LayoutToLayoutTranslator.h"
#include "SpliceParts.h"
#include "FlexiRankXlator.h"

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout1,typename Layout2>
struct SEdger : ChangeTranslator<Layout1,Layout2> {
	LayoutToLayoutTranslator<Layout1,Layout2,true> actions_;
	SEdger(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2) 
		: ChangeTranslator<Layout1,Layout2>(world1,world2) 
	{}
	typedef FlexiRankXlator<Layout2> RankXlator;
	typedef enum {spliced,one,none} SpliceState;
	SpliceState getCurrentSpliceState(typename Layout1::Edge *e1) {
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		if(Q2.whole->fetch_edge(gd<Name>(e1)))
			return one;
		else if(*SpliceEdgePartsIterator<Layout2>(Q2.current,gd<Name>(e1),'e',1))
			return spliced;
		else return none;
	}
	SpliceState getDesiredSplicedness(typename Layout1::Edge *e1) {
		if(LinkedChangeProcessor<Layout1>::world_->Q_.delE.find(e1))
			return none;
		else if(getEdgeDirection(e1)!=forward && (gd<EdgeGeom>(e1).s_edge || gd<GraphGeom>(e1->g).s_edges))
			return spliced;
		else
			return one;
	}

	void updateTailFlowRank(typename Layout2::Node *n2tf,typename Layout1::Edge *e1,int ofs) {
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;

	}
	void updateHeadFlowRank(typename Layout2::Node *n2hf,typename Layout1::Edge *e1,int ofs) {
		gd<NSRankerNode>(n2hf).newTopRank = gd<NSRankerNode>(n2hf).newBottomRank = gd<NSRankerNode>(e1->head).newTopRank - ofs;
	}
	void eraseSplice(typename Layout1::Edge *e1) {
		Name ename = gd<Name>(e1);
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		typename Layout2::Edge *e2;
		for(SpliceEdgePartsIterator<Layout2> ei2(Q2.whole,ename,'e'); e2=*ei2; ++ei2)
			actions_.DeleteEdge(e1,Q2.DelEdge(e2).object);
		typename Layout2::Node *n2;
		for(SpliceNodePartsIterator<Layout2> ni2(Q2.whole,ename,'e'); n2=*ni2; ++ni2)
			Q2.DelNode(n2); 
	}
	void createSplice(typename Layout1::Edge *e1,ChangeQueue<Layout2> &Q2) {
		Name ename = gd<Name>(e1);
		typename Layout2::Node *n2t = Q2.whole->fetch_node(gd<Name>(e1->tail)),
			*n2h = Q2.whole->fetch_node(gd<Name>(e1->head));
		typename Layout2::Node *n2tf = Q2.whole->fetch_node(partName(ename,'e',1),true).first,
			*n2hf = Q2.whole->fetch_node(partName(ename,'e',2),true).first;
		typename Layout2::Edge *e2t = Q2.whole->fetch_edge(n2t,n2tf,partName(ename,'e',1),true).first,
			*e2b = Q2.whole->fetch_edge(n2tf,n2hf,partName(ename,'e',2),true).first,
			*e2h = Q2.whole->fetch_edge(n2hf,n2h,partName(ename,'e',3),true).first;
		actions_.InsertNode(e1->tail,Q2.InsNode(n2tf).object);
		actions_.InsertNode(e1->head,Q2.InsNode(n2hf).object);
		actions_.InsertEdge(e1,Q2.InsEdge(e2t).object);
		actions_.InsertEdge(e1,Q2.InsEdge(e2b).object);
		actions_.InsertEdge(e1,Q2.InsEdge(e2h).object);
		gd<NodeGeom>(n2tf).region.Clear();
		gd<NodeGeom>(n2hf).region.Clear();
		gd<NodeGeom>(n2tf).flow = gd<NodeGeom>(n2hf).flow = 1.;
	}
	void updateRepresentation(typename Layout1::Edge *e1,int thirrank) {
		Name ename = gd<Name>(e1);
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		SpliceState splicedness = getCurrentSpliceState(e1);
		typename Layout2::Node *n2t = Q2.whole->fetch_node(gd<Name>(e1->tail)),
			*n2h = Q2.whole->fetch_node(gd<Name>(e1->head));
		switch(getDesiredSplicedness(e1)) {
		case spliced: {
			switch(splicedness) {
			case spliced:
				break;
			case one:
				actions_.DeleteEdge(e1,Q2.DelEdge(Q2.whole->fetch_edge(ename)).object);
			case none: 
				createSplice(e1,Q2);
			}
			typename Layout2::Node *n2tf = Q2.whole->fetch_node(partName(ename,'e',1)),
				*n2hf = Q2.whole->fetch_node(partName(ename,'e',2));
			typename Layout2::Edge *e2ts = Q2.whole->fetch_edge(partName(ename,'e',1)),
				*e2bs = Q2.whole->fetch_edge(partName(ename,'e',2)),
				*e2hs = Q2.whole->fetch_edge(partName(ename,'e',3));
			int tfr = gd<NSRankerNode>(e1->tail).newBottomRank + thirrank,
				hfr = gd<NSRankerNode>(e1->head).newTopRank - thirrank;
			bool mvtf,mvhf;
			mvtf = assign(gd<NSRankerNode>(n2tf).newTopRank,tfr);
			mvtf |= assign(gd<NSRankerNode>(n2tf).newBottomRank,tfr); // split to force diligent evaluation (?)
			mvhf = assign(gd<NSRankerNode>(n2hf).newTopRank,hfr);
			mvhf |= assign(gd<NSRankerNode>(n2hf).newBottomRank,hfr);
			if(mvtf) {
				ModifyNode(Q2,n2tf,DG_UPD_MOVE);
				ModifyEdge(Q2,e2ts,DG_UPD_MOVE);
			}
			if(mvhf) {
				ModifyNode(Q2,n2hf,DG_UPD_MOVE);
				ModifyEdge(Q2,e2ts,DG_UPD_MOVE);
			}
			if(mvtf||mvhf)
				ModifyEdge(Q2,e2bs,DG_UPD_MOVE);
			if(assign(gd<NodeGeom>(n2tf).suppressed,gd<NodeGeom>(n2t).suppressed))
				ModifyNode(Q2,n2tf,DG_UPD_SUPPRESSION);
			if(assign(gd<NodeGeom>(n2hf).suppressed,gd<NodeGeom>(n2h).suppressed))
				ModifyNode(Q2,n2hf,DG_UPD_SUPPRESSION);
			gd<NSRankerEdge>(e2ts).secondOfTwo = gd<NSRankerEdge>(e2bs).secondOfTwo = gd<NSRankerEdge>(e2hs).secondOfTwo = false;
			break;
		}
		case one: {
			typename Layout2::Edge *e2;
			switch(splicedness) {
			case one:
				e2 = Q2.whole->fetch_edge(ename);
				actions_.ModifyEdge(e1,Q2.ModEdge(e2).object);
				break;
			case spliced: 
				eraseSplice(e1);
			case none: 
				e2 = Q2.whole->fetch_edge(n2t,n2h,ename,true).first;
				actions_.InsertEdge(e1,Q2.InsEdge(e2).object);
				break;
			}
			if(gd<NSRankerEdge>(e1).secondOfTwo) {
				ChangeQueue<Layout1> &Q1 = LinkedChangeProcessor<Layout1>::world_->Q_;
				typename Layout1::Edge *be1 = Q1.whole->find_edge(e1->head,e1->tail);
				gd<NSRankerEdge>(e2).secondOfTwo = getDesiredSplicedness(be1)==one;
			}
			break;
		}
		case none:
			switch(splicedness) {
			case none: 
				break;
			case spliced: 
				eraseSplice(e1);
				break;
			case one: {
				typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(e1));
				actions_.DeleteEdge(e1,Q2.DelEdge(e2,false).object);
				break;
			}
			}
			break;
		}
	}
	void Process() {
		ChangeQueue<Layout1> &Q1 = LinkedChangeProcessor<Layout1>::world_->Q_;
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		actions_.ModifyGraph(Q1.ModGraph(),Q2.ModGraph());
		int thirrank = RankXlator::HeightToDRank(&LinkedChangeProcessor<Layout2>::world_->whole_,gd<GraphGeom>(&LinkedChangeProcessor<Layout2>::world_->whole_).separation.y/3.);
		for(typename Layout1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),true).first) {
				typename Layout2::Node *n2i = Q2.InsNode(n2,false).object;
				actions_.InsertNode(*ni,n2i);
			}
		for(typename Layout1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout1::Node *n1 = Q1.current->find(*ni);
				typename Layout2::Node *n2m = Q2.ModNode(n2).object;
				actions_.ModifyNode(*ni,n2m);
				for(typename Layout1::outedge_iter ei = n1->outs().begin(); ei!=n1->outs().end(); ++ei)
					if(!Q1.insE.find(*ei) && !Q1.modE.find(*ei) && !Q1.delE.find(*ei)) 
						updateRepresentation(*ei,thirrank);
				for(typename Layout1::inedge_iter ei = n1->ins().begin(); ei!=n1->ins().end(); ++ei)
					if(!Q1.insE.find(*ei) && !Q1.modE.find(*ei) && !Q1.delE.find(*ei)) 
						updateRepresentation(*ei,thirrank);
			}
		for(typename Layout1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei)
			updateRepresentation(*ei,thirrank);
		for(typename Layout1::graphedge_iter ei = Q1.modE.edges().begin(); ei!=Q1.modE.edges().end(); ++ei)
			updateRepresentation(*ei,thirrank);
		for(typename Layout1::graphedge_iter ei = Q1.delE.edges().begin(); ei!=Q1.delE.edges().end(); ++ei)
			updateRepresentation(*ei,thirrank);
		for(typename Layout1::node_iter ni = Q1.delN.nodes().begin(); ni!=Q1.delN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2d = Q2.DelNode(n2,false).object;
				actions_.DeleteNode(*ni,n2d);
			}
		LinkedChangeProcessor<Layout2>::NextProcess();
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // SEdger_h
