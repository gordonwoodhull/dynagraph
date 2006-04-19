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

#ifndef Optimizer_h
#define Optimizer_h

namespace Dynagraph {
namespace DynaDAG {

struct Optimizer {
	virtual ~Optimizer() {}
	virtual void Reorder(DynaDAGLayout &nodes,DynaDAGLayout &edges) = 0;
	virtual double Reopt(DDModel::Node *n,UpDown dir) = 0;
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Optimizer_h
