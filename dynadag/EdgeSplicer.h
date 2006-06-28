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

namespace Dynagraph {
namespace DynaDAG {

struct EdgeSplicerEndsDontMatch : DGException {
	EdgeSplicerEndsDontMatch() : DGException("Attempting to splice together two edge splines that don't share an endpoint") {}
};

template<typename Layout1,typename Layout2>
struct EdgeSplicer : ChangeTranslator<Layout1,Layout2> {
	LayoutToLayoutTranslator<Layout1,Layout2> actions_;
	EdgeSplicer(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2) 
		: ChangeTranslator<Layout1,Layout2>(world1,world2) {}
	void Process() {
		ChangeQueue<Layout1> &Q1 = LinkedChangeProcessor<Layout1>::world_->Q_;
		ChangeQueue<Layout2> &Q2 = LinkedChangeProcessor<Layout2>::world_->Q_;
		actions_.ModifyGraph(Q1.ModGraph(),Q2.ModGraph());

		// any node or edge that matches by name, we just copy layout
		for(typename Layout1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2i = Q2.InsNode(n2,false).object;
				actions_.InsertNode(*ni,n2i);
			}
		for(typename Layout1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei)
			if(Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {	
				typename Layout2::Edge *e2i = Q2.InsEdge(e2,false).object;
				actions_.InsertEdge(*ei,e2i);
			}
		for(typename Layout1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2m = Q2.ModNode(n2).object;
				actions_.ModifyNode(*ni,n2m);
			}
		for(typename Layout1::graphedge_iter ei = Q1.modE.edges().begin(); ei!=Q1.modE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2m = Q2.ModEdge(e2).object;
				actions_.ModifyEdge(*ei,e2m);
			}
		for(typename Layout1::graphedge_iter ei = Q1.delE.edges().begin(); ei!=Q1.delE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2d = Q2.DelEdge(e2,false).object;
				actions_.DeleteEdge(*ei,e2d);
			}
		for(typename Layout1::node_iter ni = Q1.delN.nodes().begin(); ni!=Q1.delN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2d = Q2.DelNode(n2,false).object;
				actions_.DeleteNode(*ni,n2d);
			}

		// next look at all current edges in the target, 
		// generate names of the parts that would be used
		// and look those up in the source to see if they've changed
		// if anything has changed, redraw that edge
		for(typename Layout2::graphedge_iter ei = Q2.current->edges().begin(); ei!=Q2.current->edges().end(); ++ei) {
			bool redraw = false;
			Name ename = gd<Name>(*ei);
			int i=0;
			while(1) { // edge parts
				++i;
				char partname[40];
				sprintf(partname,"%s_epart%d",ename.c_str(),i);
				if(typename Layout1::Edge *e = Q1.whole->fetch_edge(partname)) {
					if(Q1.insE.find(e) || Q1.modE.find(e) || Q1.delE.find(e)) {
						redraw = true;
						break;
					}
				}
				else break;
			}
			if(!redraw) {
				i=0;
				while(1) { // node parts
					++i;
					char partname[40];
					sprintf(partname,"%s_epart%d",ename.c_str(),i);
					if(typename Layout1::Node *n = Q1.whole->fetch_node(partname,false).first) {
						if(Q1.insN.find(n) || Q1.modN.find(n) || Q1.delN.find(n)) {
							redraw = true;
							break;
						}
					}
					else break;
				}
			}
			if(redraw) {
				redrawCompoundEdge(*ei);
				ModifyEdge(Q2,*ei,DG_UPD_MOVE);
			}
		}
		LinkedChangeProcessor<Layout2>::NextProcess();
	}
	void redrawCompoundEdge(typename Layout2::Edge *e) {
		Name ename = gd<Name>(e);
		EdgeGeom &eg = gd<EdgeGeom>(e);
		eg.pos.Clear();
		eg.pos.degree = 3;
		int i=0;
		while(1) { // edge parts
			++i;
			char partname[40];
			sprintf(partname,"%s_epart%d",ename.c_str(),i);
			if(typename Layout1::Edge *e1 = LinkedChangeProcessor<Layout1>::world_->whole_.fetch_edge(partname)) {
				EdgeGeom &eg1 = gd<EdgeGeom>(e1);
				Line::iterator begin = eg1.pos.begin();
				if(eg.pos.size()) {
					if(eg.pos.back()!=*begin)
						throw EdgeSplicerEndsDontMatch();
					++begin;
				}
				eg.pos.insert(eg.pos.end(),begin,eg1.pos.end());
			}
			else break;
		}
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // EdgeSplicer_h
