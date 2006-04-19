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

#ifndef PathOptim_h
#define PathOptim_h

namespace Dynagraph {
namespace DynaDAG {

// the classic DynaDAG path optimizer
struct PathOptim : Optimizer {
	PathOptim(Config &config) : config(config) {}
	void Reorder(DynaDAGLayout &nodes,DynaDAGLayout &edges);
	double Reopt(DDModel::Node *n,UpDown dir);
private:
	Config &config;
	void optPath(DDPath *path);
	bool leftgoing(DDModel::Node *n, UpDown dir, int eq_pass);
	void shiftLeft(DDModel::Node *n);
	bool rightgoing(DDModel::Node *n, UpDown dir, int eq_pass);
	void shiftRight(DDModel::Node *n);
	double coordBetween(DDModel::Node *L, DDModel::Node *R);
	void resetCoord(DDModel::Node *n);
	void optElt(DDModel::Node *n, UpDown dir, int eq_pass);
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // PathOptim_h
