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

#ifndef ConstraintGraph_h
#define ConstraintGraph_h

#include "ns.h"

namespace Dynagraph {
namespace DynaDAG {

// dynadag constraint graphs: basic data + debug accounting
struct ConstraintType {
	enum {unknown,anchor,stab,node,rankWeak,orderEdgeStraighten} why;
};
struct DDCNodeData : NS::NSNode<void*,void*>, ConstraintType {};

template<bool AllowParallel>
struct DDCGraph {
	typedef LGraph<ADTisCDT,AllowParallel,NS::NSData<void*,void*>,DDCNodeData,NS::NSEdge<void*,void*> > Graph;
};

template<bool AllowParallel>
struct NSCG : NS::NS<typename DDCGraph<AllowParallel>::Graph,NS::AccessNoAttr<typename DDCGraph<AllowParallel>::Graph > > {};

typedef NSCG<true> LlelNS;
typedef NSCG<false> UniqNS;

template<bool AllowParallel>
struct ConstraintGraph : DDCGraph<AllowParallel>::Graph {
	typename ConstraintGraph::Node *anchor;
	typedef NSCG<AllowParallel> MyNS;

	struct NodeConstraints {
		typename ConstraintGraph::Node *n, // the variable
			*stab; // for stability constraints
		NodeConstraints() : n(0),stab(0) {}
		~NodeConstraints() {
			dgassert(empty());
		}
		void clear() {
			n = stab = 0;
		}
		bool empty() {
			return !n && !stab;
		}
	};

	struct NSEdgePair {
		typename ConstraintGraph::Edge *e[2];
		NSEdgePair(typename ConstraintGraph::Node *anchor,
				typename ConstraintGraph::Node *tail,
				typename ConstraintGraph::Node *head) {
			e[0] = anchor->g->fiat_edge(anchor,tail);
			e[1] = anchor->g->fiat_edge(anchor,head);
		}
	};

	ConstraintGraph() : anchor(0) {
		anchor = this->create_node();
		gd<ConstraintType>(anchor).why = ConstraintType::anchor;
	}
	typename ConstraintGraph::Node *GetVar(NodeConstraints &nc) {
		if(!nc.n)
			gd<ConstraintType>(nc.n = this->create_node()).why = ConstraintType::node;
		return nc.n;
	}
	void Stabilize(NodeConstraints &nc, int newrank, int weight) {
		if(!nc.stab)
			gd<ConstraintType>(nc.stab = this->create_node()).why = ConstraintType::stab;
	//	dgassert(newrank>-1000000 && newrank<1000000);
		typename ConstraintGraph::Node *var = GetVar(nc);
		int len0,len1;
		if (newrank >= 0) {
			len0 = 0;
			len1 = newrank;
		}
		else {
			len0 = -newrank;
			len1 = 0;
		}
		NSEdgePair ep(nc.stab,anchor,var);
		MyNS::NSd(ep.e[0]).minlen = len0;
		MyNS::NSd(ep.e[1]).minlen = len1;
		MyNS::NSd(ep.e[0]).weight = weight;
		MyNS::NSd(ep.e[1]).weight = weight;
	}
	void Unstabilize(NodeConstraints &nc) {
		if(nc.stab) {
			erase_node(nc.stab);
			nc.stab = 0;
		}
		/*if (nd->con[csys].n)
			agdelete(cg,nd->con[csys].n); */ 	/* incorrect? */
	}
	void RemoveNodeConstraints(NodeConstraints &nc) {
		if(nc.n) {
			erase_node(nc.n);
			nc.n = 0;
		}
		if(nc.stab) {
			erase_node(nc.stab);
			nc.stab = 0;
		}
	}
};
typedef ConstraintGraph<true> LlelConstraintGraph;
typedef ConstraintGraph<false> UniqConstraintGraph;

} // namespace DynaDAG
} // namespace Dynagraph

#endif // ConstraintGraph_h
