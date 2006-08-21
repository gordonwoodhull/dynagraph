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


#ifndef EdgeSplicer_h
#define EdgeSplicer_h

#include "common/LayoutToLayoutTranslator.h"
#include "SpliceParts.h"

namespace Dynagraph {
namespace DynaDAG {

struct EdgeSplicerEndsDontMatch : DGException {
	EdgeSplicerEndsDontMatch() : DGException("attempt to splice together two edge splines that don't share an endpoint",true) {}
};

template<typename Layout1,typename Layout2>
struct EdgeSplicer : ChangeTranslator<Layout1,Layout2> {
	LayoutToLayoutTranslator<Layout1,Layout2,false> actions_;
	EdgeSplicer(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2)
		: ChangeTranslator<Layout1,Layout2>(world1,world2) {}
	void Process() {
		ChangeQueue<Layout1> &Q1 = LinkedChangeProcessor<Layout1>::world_->Q_;
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		actions_.ModifyGraph(Q1.ModGraph(),Q2.ModGraph());

		// any node or edge that matches by name, we just copy layout
		for(typename Layout1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni))) {
				typename Layout2::Node *n2i = Q2.InsNode(n2,false).object;
				actions_.InsertNode(*ni,n2i);
			}
		for(typename Layout1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2i = Q2.InsEdge(e2,false).object;
				actions_.InsertEdge(*ei,e2i);
			}
		for(typename Layout1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni))) {
				typename Layout2::Node *n2m = Q2.ModNode(n2).object;
				actions_.ModifyNode(*ni,n2m);
			}
		for(typename Layout1::graphedge_iter ei = Q1.modE.edges().begin(); ei!=Q1.modE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2m = Q2.ModEdge(e2).object;
				actions_.ModifyEdge(*ei,e2m);
			}
		for(typename Layout1::graphedge_iter ei = Q1.delE.edges().begin(); ei!=Q1.delE.edges().end(); ++ei) {
			if(*SpliceEdgePartsIterator<Layout1>(Q1.current,gd<Name>(*ei),'e'))
				continue; // this is now a spliced edge - not really a delete
			if(typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2d = Q2.DelEdge(e2,false).object;
				actions_.DeleteEdge(*ei,e2d);
			}
		}
		for(typename Layout1::node_iter ni = Q1.delN.nodes().begin(); ni!=Q1.delN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni))) {
				typename Layout2::Node *n2d = Q2.DelNode(n2,false).object;
				actions_.DeleteNode(*ni,n2d);
			}

		// next look at all current edges in the target,
		// generate names of the parts that would be used
		// and look those up in the source to see if they've changed
		// if anything has changed, redraw that edge
		for(typename Layout2::graphedge_iter ei2 = Q2.current->edges().begin(); ei2!=Q2.current->edges().end(); ++ei2) {
			if(Q1.current->fetch_edge(gd<Name>(*ei2)))
				continue; // this is an unspliced edge - ignore splice parts if any
			bool redraw = false;
			Name ename = gd<Name>(*ei2);
			typename Layout1::Edge *e;
			for(SpliceEdgePartsIterator<Layout1> ei1(Q1.whole,ename,'e'); (e=*ei1); ++ei1)
				if(Q1.insE.find(e) || Q1.modE.find(e) || Q1.delE.find(e)) {
					redraw = true;
					break;
				}
			if(!redraw) {
				typename Layout1::Node *n;
				for(SpliceNodePartsIterator<Layout1> ni1(Q1.whole,ename,'e'); (n=*ni1); ++ni1)
					if(Q1.insN.find(n) || Q1.modN.find(n) || Q1.delN.find(n)) {
						redraw = true;
						break;
					}
			}
			if(redraw) {
				redrawCompoundEdge(*ei2);
				ModifyEdge(Q2,*ei2,DG_UPD_MOVE);
			}
		}
		LinkedChangeProcessor<Layout2>::NextProcess();
	}
	void redrawCompoundEdge(typename Layout2::Edge *e) {
		Name ename = gd<Name>(e);
		EdgeGeom &eg2 = gd<EdgeGeom>(e);
		eg2.pos.Clear();
		eg2.pos.degree = 3;
		typename Layout1::Edge *e1;
		for(SpliceEdgePartsIterator<Layout1> ei1(&LinkedChangeProcessor<Layout1>::world_->current_,ename,'e'); (e1=*ei1);++ei1) {
			EdgeGeom &eg1 = gd<EdgeGeom>(e1);
			if(eg1.pos.size()) {
				Line::iterator begin = eg1.pos.begin();
				if(eg2.pos.size()) {
					if(eg2.pos.back()!=*begin)
						throw EdgeSplicerEndsDontMatch();
					++begin;
				}
				eg2.pos.insert(eg2.pos.end(),begin,eg1.pos.end());
			}
		}
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // EdgeSplicer_h
