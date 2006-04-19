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

#ifndef DotlikeOptimizer_h
#define DotlikeOptimizer_h

namespace Dynagraph {
namespace DynaDAG {

struct DotlikeOptimizer : Optimizer {
	DotlikeOptimizer(Config &config) : config(config) {}
	void Reorder(DynaDAGLayout &nodes,DynaDAGLayout &edges);
	double Reopt(DDModel::Node *n,UpDown dir);
private:
	Config &config;
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // DotlikeOptimizer_h
