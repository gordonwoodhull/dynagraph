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

#ifndef Constraints_h
#define Constraints_h

#include "ns.h"

namespace Dynagraph {
namespace DynaDAG {

// dynadag constraint graphs: basic data + debug accounting
struct ConstraintType {
	enum {unknown,anchor,stab,node,rankWeak,orderEdgeStraighten} why;
};
struct DDCNodeData : NS::NSNode<void*,void*>, ConstraintType {};
typedef LGraph<ADTisCDT,NS::NSData<void*,void*>,DDCNodeData,NS::NSEdge<void*,void*> > DDCGraph;
typedef NS::NS<DDCGraph,NS::AccessNoAttr<DDCGraph> > DDNS;

struct NodeConstraints {
	DDCGraph::Node *n, // the variable
		*stab; // for stability constraints
	NodeConstraints() : n(0),stab(0) {}
	~NodeConstraints() {
		assert(empty());
	}
	void clear() {
		n = stab = 0;
	}
	bool empty() {
		return !n && !stab;
	}
};

struct NSEdgePair {
	DDCGraph::Edge *e[2];
	NSEdgePair(DDCGraph::Node *anchor,DDCGraph::Node *tail,DDCGraph::Node *head) {
		e[0] = anchor->g->create_edge(anchor,tail).first;
		e[1] = anchor->g->create_edge(anchor,head).first;
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Constraints_h
