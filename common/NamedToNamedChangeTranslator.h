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

#include "ChangeTranslator.h"

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
		ChangeQueue<Graph1> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<Graph2> &destQ = this->destWorld_->Q_;
		actions_.OpenGraph(srcQ.ModGraph(),destQ.ModGraph());
	}
	virtual void Process() {
		ChangeQueue<Graph1> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<Graph2> &destQ = this->destWorld_->Q_;
		actions_.ModifyGraph(srcQ.ModGraph(),destQ.ModGraph());
		for(typename Graph1::node_iter ni = srcQ.insN.nodes().begin(); ni!=srcQ.insN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = destQ.whole->fetch_node(gd<Name>(*ni),true);
			typename Graph2::Node *n = destQ.InsNode(nb2.first,CheckRedundancy).object;
			actions_.InsertNode(*ni,n);
		}
		for(typename Graph1::graphedge_iter ei = srcQ.insE.edges().begin(); ei!=srcQ.insE.edges().end(); ++ei) {
			std::pair<typename Graph2::Edge *,bool> eb2 = destQ.whole->fetch_edge(gd<Name>((*ei)->tail),gd<Name>((*ei)->head),gd<Name>(*ei),true);
			typename Graph2::Edge *e = destQ.InsEdge(eb2.first,CheckRedundancy).object;
			actions_.InsertEdge(*ei,e);
		}
		for(typename Graph1::node_iter ni = srcQ.modN.nodes().begin(); ni!=srcQ.modN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = destQ.whole->fetch_node(gd<Name>(*ni),false);
			typename Graph2::Node *n = destQ.ModNode(nb2.first).object;
			actions_.ModifyNode(*ni,n);
		}
		for(typename Graph1::graphedge_iter ei = srcQ.modE.edges().begin(); ei!=srcQ.modE.edges().end(); ++ei) {
			typename Graph2::Edge *e2 = destQ.whole->fetch_edge(gd<Name>(*ei));
			if(!e2)
				throw EdgeDoesntExistInconsistency(gd<Name>(*ei));
			typename Graph2::Edge *e = destQ.ModEdge(e2).object;
			actions_.ModifyEdge(*ei,e);
		}
		for(typename Graph1::graphedge_iter ei = srcQ.delE.edges().begin(); ei!=srcQ.delE.edges().end(); ++ei) {
			typename Graph2::Edge *e2 = destQ.whole->fetch_edge(gd<Name>(*ei));
			if(!e2)
				throw EdgeDoesntExistInconsistency(gd<Name>(*ei));
			typename Graph2::Edge *e = destQ.DelEdge(e2,CheckRedundancy).object;
			actions_.DeleteEdge(*ei,e);
		}
		for(typename Graph1::node_iter ni = srcQ.delN.nodes().begin(); ni!=srcQ.delN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = destQ.whole->fetch_node(gd<Name>(*ni),false);
			if(nb2.second)
				throw NodeDoesntExistInconsistency(gd<Name>(*ni));
			typename Graph2::Node *n = destQ.DelNode(nb2.first,CheckRedundancy).object;
			actions_.DeleteNode(*ni,n);
		}
	}
};

} // namespace Dynagraph

#endif // NamedToNamedChangeTranslator_h
