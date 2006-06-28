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

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout2>
inline void eraseSplice(ChangeQueue<Layout2> &Q,Name ename) {
	typename Layout2::Node *n2;
	for(SpliceNodePartsIterator ni2(Q.whole,ename,'e'); n2=*ni2; ++ni2)
		Q2.DelNode(n2); // and edges deleted by implication
	// this does not call any DeleteNode or DeleteEdge translator actions but those are empty (?)
}

template<typename Layout1,typename Layout2>
struct SEdger : ChangeTranslator<Layout1,Layout2> {
	LayoutToLayoutTranslator<Layout1,Layout2> actions_;
	SEdger(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2) 
		: ChangeTranslator<Layout1,Layout2>(world1,world2) 
	{}
	typedef enum {spliced,one,none} SpliceState;
	SpliceState getSpliceState(typename Layout1::Edge *e1) {
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		if(Q2.whole->fetch_edge(gd<Name>(e1)))
			return one;
		else if(*SpliceEdgePartsIterator(Q2.current,gd<Name>(e1),'e'))
			return spliced;
		else return none;
	}

	void updateRepresentation(typename Layout1::Edge *e1) {
		Name ename = gd<Name>(e1);
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		SpliceState splicedness = getSpliceState(e1);
		if(gd<NSRankerEdge>(e1).direction==NSRankerEdge::reversed || gd<NSRankerEdge>(e1).direction == NSRankerEdge::flat) {
			switch(splicedness) {
			case spliced:
				break;
			case one:
				actions_.DeleteEdge(e1,Q2.DelEdge(Q2.whole->fetch_edge(ename));
			case none: {
				typename Layout2::Node *n2t = Q2.whole->fetch_node(gd<Name>(e1->tail)),
					*n2h = Q2.whole->fetch_node(gd<Name>(e1->head));
				typename Layout2::Node *n2tf = Q2.whole->fetch_node(partName(ename,'e',1),true).first,
					*n2hf = Q2.whole->fetch_node(partName(ename,'e',2),true).first;
				typename Layout2::Edge *e2t = Q2.whole->fetch_edge(n2t,n2tf,partName(ename,'e',1),true).first,
					*e2b = Q2.whole->fetch_edge(n2tf,n2hf,partName(ename,'e',2),true).first,
					*e2h = Q2.whole->fetch_edge(n2hf,n2h,partName(ename,'e',3),true).first;
				actions_.InsertNode(e1->tail,Q2.InsNode(n2tf));
				actions_.InsertNode(e1->head,Q2.InsNode(n2hf));
				actions_.InsertEdge(e1,Q2.InsEdge(e2t));
				actions_.InsertEdge(e1,Q2.InsEdge(e2b));
				actions_.InsertEdge(e1,Q2.InsEdge(e2h));
			}
			}
			typename Layout2::Node *n2tf = Q2.whole->fetch_node(partName(ename,'e',1)),
				*n2hf = Q2.whole->fetch_node(partName(ename,'e',2));
			gd<NSRankerNode>(n2tf).newTopRank = gd<NSRankerNode>(n2tf).newBottomRank = gd<NSRankerNode>(e1->tail).newBottomRank + thirrank;
			gd<NSRankerNode>(n2hf).newTopRank = gd<NSRankerNode>(n2hf).newBottomRank = gd<NSRankerNode>(e1->head).newTopRank - thirrank;
		}
		else { // direction==forward
			switch(splicedness) {
			case spliced: 
				eraseSplice(Q2,ename);
			case none: {
				typename Layout2::Node *n2t = Q2.whole->fetch_node(gd<Name>(e1->tail)),
					*n2h = Q2.whole->fetch_node(gd<Name>(e1->head));
				typename Layout2::Edge *e2 = Q2.whole->fetch_edge(n2t,n2h,ename,true).first;
				actions_.InsertEdge(e1,Q2.InsEdge(e2));
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
		for(typename Layout1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2i = Q2.InsNode(n2,false).object;
				actions_.InsertNode(*ni,n2i);
			}
		for(typename Layout1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei)
			updateRepresentation(*ei);
		for(typename Layout1::graphedge_iter ei = Q1.modE.edges().begin(); ei!=Q1.modE.edges().end(); ++ei)
			updateRepresentation(*ei);
		for(typename Layout1::graphedge_iter ei = Q1.delE.edges().begin(); ei!=Q1.delE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2d = Q2.DelEdge(e2,false).object;
				actions_.DeleteEdge(*ei,e2d);
			}
			else eraseSplice(Q2,gd<Name>(*ei_));

		for(typename Layout1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2m = Q2.ModNode(n2).object;
				actions_.ModifyNode(*ni,n2m);
				for(typename Layout1::outedge_iter ei = (*ni)->outs().begin(); ei!=(*ni)->outs().end(); ++ei)
					if(!Q1.insE.find(*ei) && !Q1.modE.find(*ei) && !Q1.delE.find(*ei)) 
						if(typename Layout2::Node *n2t = *SpliceNodePartsIterator(Q2.whole,gd<Name>(*ei),'e',1)) {
							if(igd<Update>(*ni).flags & DG_UPD_SUPPRESS2ION) 
								gd<NodeGeom>(n2t).suppressed = gd<NodeGeom>(*ni).suppressed;
							if(igd<Update>(*ni).flags & DG_UPD_SUPPRESSION) 
							r(SpliceNodePartsIterator ni2(
			}
		for(typename Layout1::node_iter ni = Q1.delN.nodes().begin(); ni!=Q1.delN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2d = Q2.DelNode(n2,false).object;
				actions_.DeleteNode(*ni,n2d);
			}
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // SEdger_h
