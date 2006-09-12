/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/

#ifndef NamedToNamedChangeTranslator_h
#define NamedToNamedChangeTranslator_h

#include "ChangeProcessor.h"

namespace Dynagraph {
struct NodeDoesntExistInconsistency : DGException2 {
	NodeDoesntExistInconsistency(DString name) : DGException2("internal inconsistency: node doesn't exist",name,true) {}
};
struct EdgeDoesntExistInconsistency : DGException2 {
	EdgeDoesntExistInconsistency(DString name) : DGException2("internal inconsistency: edge doesn't exist",name,true) {}
};
template<typename Graph1,typename Graph2,typename ChangeActions,bool CheckRedundancy>
struct NamedToNamedChangeTranslator : ChangeTranslator<Graph1,Graph2> {
	ChangeActions actions_;
	NamedToNamedChangeTranslator(ChangingGraph<Graph1> *world1,ChangingGraph<Graph2> *world2,const ChangeActions &action = ChangeActions()) 
		: ChangeTranslator<Graph1,Graph2>(world1,world2),actions_(action) {}
	virtual void Open() {
		// graph open is like node/edge insert in that changes don't get marked
		// so use Mod code and then clear flags!
		ChangeQueue<Graph1> &Q1 = LinkedChangeProcessor<Graph1>::world_->Q_;
		ChangeQueue<Graph2> &Q2 = LinkedChangeProcessor<Graph2>::world_->Q_;
		actions_.ModifyGraph(Q1.ModGraph(),Q2.ModGraph());
		igd<Update>(Q2.ModGraph()).flags = 0;
		LinkedChangeProcessor<Graph2>::NextOpen();
	}
	virtual void Process() {
		ChangeQueue<Graph1> &Q1 = LinkedChangeProcessor<Graph1>::world_->Q_;
		ChangeQueue<Graph2> &Q2 = LinkedChangeProcessor<Graph2>::world_->Q_;
		actions_.ModifyGraph(Q1.ModGraph(),Q2.ModGraph());
		for(typename Graph1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = Q2.whole->fetch_node(gd<Name>(*ni),true);
			typename Graph2::Node *n = Q2.InsNode(nb2.first,CheckRedundancy).object;
			actions_.InsertNode(*ni,n);
		}
		for(typename Graph1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei) {
			std::pair<typename Graph2::Edge *,bool> eb2 = Q2.whole->fetch_edge(gd<Name>((*ei)->tail),gd<Name>((*ei)->head),gd<Name>(*ei),true);
			typename Graph2::Edge *e = Q2.InsEdge(eb2.first,CheckRedundancy).object;
			actions_.InsertEdge(*ei,e);
		}
		for(typename Graph1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = Q2.whole->fetch_node(gd<Name>(*ni),false);
			typename Graph2::Node *n = Q2.ModNode(nb2.first).object;
			actions_.ModifyNode(*ni,n);
		}
		for(typename Graph1::graphedge_iter ei = Q1.modE.edges().begin(); ei!=Q1.modE.edges().end(); ++ei) {
			typename Graph2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei));
			if(!e2)
				throw EdgeDoesntExistInconsistency(gd<Name>(*ei));
			typename Graph2::Edge *e = Q2.ModEdge(e2).object;
			actions_.ModifyEdge(*ei,e);
		}
		for(typename Graph1::graphedge_iter ei = Q1.delE.edges().begin(); ei!=Q1.delE.edges().end(); ++ei) {
			typename Graph2::Edge *e2 = Q2.whole->fetch_edge(gd<Name>(*ei));
			if(!e2)
				throw EdgeDoesntExistInconsistency(gd<Name>(*ei));
			typename Graph2::Edge *e = Q2.DelEdge(e2,CheckRedundancy).object;
			actions_.DeleteEdge(*ei,e);
		}
		for(typename Graph1::node_iter ni = Q1.delN.nodes().begin(); ni!=Q1.delN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = Q2.whole->fetch_node(gd<Name>(*ni),false);
			if(nb2.second)
				throw NodeDoesntExistInconsistency(gd<Name>(*ni));
			typename Graph2::Node *n = Q2.DelNode(nb2.first,CheckRedundancy).object;
			actions_.DeleteNode(*ni,n);
		}
		LinkedChangeProcessor<Graph2>::NextProcess();
	}
};

} // namespace Dynagraph

#endif // NamedToNamedChangeTranslator_h
