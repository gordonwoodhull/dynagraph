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

namespace Dynagraph {
namespace DynaDAG {

struct ConstraintGraph : DDCGraph {
	Node *anchor;

	ConstraintGraph();
	Node *GetVar(NodeConstraints &nc);
	void Stabilize(NodeConstraints &nc, int newrank, int weight);
	void Unstabilize(NodeConstraints &nc);
	void RemoveNodeConstraints(NodeConstraints &nc);
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // ConstraintGraph_h
