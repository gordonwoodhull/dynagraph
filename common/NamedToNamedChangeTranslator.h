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
	NodeDoesntExistInconsistency(DString name) : DGException2("Internal inconsistency: node doesn't exist",name,true) {}
};
struct EdgeDoesntExistInconsistency : DGException2 {
	EdgeDoesntExistInconsistency(DString name) : DGException2("Internal inconsistency: edge doesn't exist",name,true) {}
};
template<typename Graph1,typename Graph2>
struct GoingNamedTransition {
	ChangeQueue<Graph2> Q_;
	GoingNamedTransition(Graph2 *world2,Graph2 *current2) : Q_(world2,current2) {}
	GoingNamedTransition(const GoingNamedTransition &other) : Q_(other.Q_.whole,other.Q_.current) {
		Q_ = other.Q_;
	}
	ChangeQueue<Graph2> &NextQ() {
		return Q_;
	}
	bool CheckRedundancy() {
		return true;
	}
	void EndLastQ(ChangeQueue<Graph1> &Q) {}
};
template<typename Graph1,typename Graph2>
struct ReturningNamedTransition {
	ChangeQueue<Graph2> *nextQ_;
	ChangeQueue<Graph2> &NextQ() {
		return *nextQ_;
	}
	bool CheckRedundancy() {
		return false;
	}
	void EndLastQ(ChangeQueue<Graph1> &Q) {
		Q.Execute(true);
	}
};
template<typename Graph1,typename Graph2,typename Transition,typename ChangeActions>
struct NamedToNamedChangeTranslator : ChangeTranslator<Graph1,Graph2> {
	Transition transition_;
	ChangeActions actions_;
	NamedToNamedChangeTranslator(const Transition &transition = Transition(),const ChangeActions &action = ChangeActions()) 
		: transition_(transition),actions_(action) {}
	virtual void Process(ChangeQueue<Graph1> &Q1) {
		ChangeQueue<Graph2> &Q2 = transition_.NextQ();
		actions_.ModifyGraph(Q1.ModGraph(),Q2.ModGraph());
		for(typename Graph1::node_iter ni = Q1.insN.nodes().begin(); ni!=Q1.insN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = Q2.whole->fetch_node(gd<Name>(*ni),true);
			typename Graph2::Node *n = Q2.InsNode(nb2.first,transition_.CheckRedundancy()).object;
			actions_.InsertNode(*ni,n);
		}
		for(typename Graph1::graphedge_iter ei = Q1.insE.edges().begin(); ei!=Q1.insE.edges().end(); ++ei) {
			std::pair<typename Graph2::Edge *,bool> eb2 = Q2.whole->fetch_edge(gd<Name>((*ei)->tail),gd<Name>((*ei)->head),gd<Name>(*ei),true);
			typename Graph2::Edge *e = Q2.InsEdge(eb2.first,transition_.CheckRedundancy()).object;
			actions_.InsertEdge(*ei,e);
		}
		for(typename Graph1::node_iter ni = Q1.modN.nodes().begin(); ni!=Q1.modN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = Q2.whole->fetch_node(gd<Name>(*ni),false);
			if(nb2.second)
				throw NodeDoesntExistInconsistency(gd<Name>(*ni));
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
			typename Graph2::Edge *e = Q2.DelEdge(e2,transition_.CheckRedundancy()).object;
			actions_.DeleteEdge(*ei,e);
		}
		for(typename Graph1::node_iter ni = Q1.delN.nodes().begin(); ni!=Q1.delN.nodes().end(); ++ni) {
			std::pair<typename Graph2::Node *,bool> nb2 = Q2.whole->fetch_node(gd<Name>(*ni),false);
			if(nb2.second)
				throw NodeDoesntExistInconsistency(gd<Name>(*ni));
			typename Graph2::Node *n = Q2.DelNode(nb2.first,transition_.CheckRedundancy()).object;
			actions_.DeleteNode(*ni,n);
		}
		transition_.EndLastQ(Q1);
		NextProcess(Q2);
	}
};

} // namespace Dynagraph

#endif // NamedToNamedChangeTranslator_h
