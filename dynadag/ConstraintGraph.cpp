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


#include "DynaDAG.h"

namespace Dynagraph {
namespace DynaDAG {

ConstraintGraph::ConstraintGraph() : anchor(create_node()) {
	gd<ConstraintType>(anchor).why = ConstraintType::anchor;
}
DDCGraph::Node *ConstraintGraph::GetVar(NodeConstraints &nc) {
	if(!nc.n)
		gd<ConstraintType>(nc.n = create_node()).why = ConstraintType::node;
	return nc.n;
}

void ConstraintGraph::Stabilize(NodeConstraints &nc, int newrank, int weight) {
	if(!nc.stab)
		gd<ConstraintType>(nc.stab = create_node()).why = ConstraintType::stab;
//	assert(newrank>-1000000 && newrank<1000000);
	DDCGraph::Node *var = GetVar(nc);
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
	DDNS::NSd(ep.e[0]).minlen = len0;
	DDNS::NSd(ep.e[1]).minlen = len1;
	DDNS::NSd(ep.e[0]).weight = weight;
	DDNS::NSd(ep.e[1]).weight = weight;
}
void ConstraintGraph::Unstabilize(NodeConstraints &nc) {
	if(nc.stab) {
		erase_node(nc.stab);
		nc.stab = 0;
	}
	/*if (nd->con[csys].n)
		agdelete(cg,nd->con[csys].n); */ 	/* incorrect? */
}
void ConstraintGraph::RemoveNodeConstraints(NodeConstraints &nc) {
	if(nc.n) {
		erase_node(nc.n);
		nc.n = 0;
	}
	if(nc.stab) {
		erase_node(nc.stab);
		nc.stab = 0;
	}
}

} // namespace DynaDAG
} // namespace Dynagraph
