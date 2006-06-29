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
	LayoutToLayoutTranslator<Layout1,Layout2> actions_;
	SEdger(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2) 
		: ChangeTranslator<Layout1,Layout2>(world1,world2) 
	{}
	typedef FlexiRankXlator<Layout2> RankXlator;
	typedef enum {spliced,one,none} SpliceState;
	SpliceState getSpliceState(typename Layout1::Edge *e1) {
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		if(Q2.whole->fetch_edge(gd<Name>(e1)))
			return one;
		else if(*SpliceEdgePartsIterator<Layout2>(Q2.current,gd<Name>(e1),'e',1))
			return spliced;
		else return none;
	}

	static void updateTailFlowRank(typename Layout2::Node *n2tf,typename Layout1::Edge *e1,int ofs) {
		gd<NSRankerNode>(n2tf).newTopRank = gd<NSRankerNode>(n2tf).newBottomRank = gd<NSRankerNode>(e1->tail).newBottomRank + ofs;
	}
	static void updateHeadFlowRank(typename Layout2::Node *n2hf,typename Layout1::Edge *e1,int ofs) {
		gd<NSRankerNode>(n2hf).newTopRank = gd<NSRankerNode>(n2hf).newBottomRank = gd<NSRankerNode>(e1->head).newTopRank - ofs;
	}
	void eraseSplice(typename Layout1::Edge *e1) {
		Name ename;
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		typename Layout2::Edge *e2;
		for(SpliceEdgePartsIterator<Layout2> ei2(Q2.whole,ename,'e'); e2=*ei2; ++ei2)
			actions_.DeleteEdge(e1,Q2.DelEdge(e2).object);
		typename Layout2::Node *n2;
		for(SpliceNodePartsIterator<Layout2> ni2(Q2.whole,ename,'e'); n2=*ni2; ++ni2)
			Q2.DelNode(n2); 
	}
	void updateRepresentation(typename Layout1::Edge *e1,int thirrank) {
		Name ename = gd<Name>(e1);
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		SpliceState splicedness = getSpliceState(e1);
		typename Layout2::Node *n2t = Q2.whole->fetch_node(gd<Name>(e1->tail)),
			*n2h = Q2.whole->fetch_node(gd<Name>(e1->head));
		if(gd<NSRankerEdge>(e1).direction==NSRankerEdge::reversed || gd<NSRankerEdge>(e1).direction == NSRankerEdge::flat) {
			switch(splicedness) {
			case spliced:
				break;
			case one:
				actions_.DeleteEdge(e1,Q2.DelEdge(Q2.whole->fetch_edge(ename)).object);
			case none: {
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
			}
			}
			typename Layout2::Node *n2tf = Q2.whole->fetch_node(partName(ename,'e',1)),
				*n2hf = Q2.whole->fetch_node(partName(ename,'e',2));
			updateTailFlowRank(n2tf,e1,thirrank);
			updateHeadFlowRank(n2hf,e1,thirrank);
			gd<NodeGeom>(n2tf).suppressed = gd<NodeGeom>(n2t).suppressed;
			gd<NodeGeom>(n2hf).suppressed = gd<NodeGeom>(n2h).suppressed;
		}
		else { // direction==forward
			switch(splicedness) {
			case spliced: 
				eraseSplice(e1);
			case none: {
				typename Layout2::Edge *e2 = Q2.whole->fetch_edge(n2t,n2h,ename,true).first;
				actions_.InsertEdge(e1,Q2.InsEdge(e2).object);
				break;
			}
			case one:
				break;
			}
		}
	}
	void Process() {
		ChangeQueue<Layout1> &Q1 = LinkedChangeProcessor<Layout1>::world_->Q_;
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		int thirrank = RankXlator::HeightToDRank(&LinkedChangeProcessor<Layout2>::world_->whole_,gd<GraphGeom>(&LinkedChangeProcessor<Layout2>::world_->whole_).separation.y/3.);
		for(typename Layout1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),true).first) {
				typename Layout2::Node *n2i = Q2.InsNode(n2,false).object;
				actions_.InsertNode(*ni,n2i);
			}
		for(typename Layout1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei)
			updateRepresentation(*ei,thirrank);
		for(typename Layout1::graphedge_iter ei = Q1.modE.edges().begin(); ei!=Q1.modE.edges().end(); ++ei)
			updateRepresentation(*ei,thirrank);
		for(typename Layout1::graphedge_iter ei = Q1.delE.edges().begin(); ei!=Q1.delE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2d = Q2.DelEdge(e2,false).object;
				actions_.DeleteEdge(*ei,e2d);
			}
			else eraseSplice(*ei);

		for(typename Layout1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2m = Q2.ModNode(n2).object;
				actions_.ModifyNode(*ni,n2m);
				for(typename Layout1::outedge_iter ei = (*ni)->outs().begin(); ei!=(*ni)->outs().end(); ++ei)
					if(!Q1.insE.find(*ei) && !Q1.modE.find(*ei) && !Q1.delE.find(*ei)) 
						if(typename Layout2::Node *n2t = *SpliceNodePartsIterator<Layout2>(Q2.whole,gd<Name>(*ei),'e',1)) {
							actions_.ModifyNode(*ni,Q2.ModNode(n2t).object);
							if(igd<Update>(*ni).flags & DG_UPD_MOVE) 
								updateTailFlowRank(n2t,*ei,thirrank);
						}
				for(typename Layout1::inedge_iter ei = (*ni)->ins().begin(); ei!=(*ni)->ins().end(); ++ei)
					if(!Q1.insE.find(*ei) && !Q1.modE.find(*ei) && !Q1.delE.find(*ei)) 
						if(typename Layout2::Node *n2h = *SpliceNodePartsIterator<Layout2>(Q2.whole,gd<Name>(*ei),'e',2)) {
							actions_.ModifyNode(*ni,Q2.ModNode(n2h).object);
							if(igd<Update>(*ni).flags & DG_UPD_MOVE) 
								updateHeadFlowRank(n2h,*ei,thirrank);
						}
			}
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
