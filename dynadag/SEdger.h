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

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout1,typename Layout2>
struct SEdger : ChangeTranslator<Layout1,Layout2> {
	LayoutToLayoutTranslator<Layout1,Layout2> actions_;
	SEdger(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2) 
		: ChangeTranslator<Layout1,Layout2>(world1,world2) {}
	void Process() {
		for(typename Layout1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2i = Q2.InsNode(n2,false).object;
				actions_.InsertNode(*ni,n2i);
			}
		for(typename Layout1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2m = Q2.ModNode(n2).object;
				actions_.ModifyNode(*ni,n2m);
			}
		for(typename Layout1::node_iter ni = Q1.delN.nodes().begin(); ni!=Q1.delN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = Q2.whole->fetch_node(gd<Name>(*ni),false).first) {
				typename Layout2::Node *n2d = Q2.DelNode(n2,false).object;
				actions_.DeleteNode(*ni,n2d);
			}

		for(typename Layout1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei)
			if(Layout2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei))) {	
				typename Layout2::Edge *e2i = Q2.InsEdge(e2,false).object;
				actions_.InsertEdge(*ei,e2i);
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
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // SEdger_h
