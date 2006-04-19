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

#ifndef Sifter_h
#define Sifter_h

namespace Dynagraph {
namespace DynaDAG {

struct Sifter : Optimizer {
	Sifter(Config &config) : config(config) {}
	void Reorder(DynaDAGLayout &nodes,DynaDAGLayout &edges);
	double Reopt(DDModel::Node *n,UpDown dir);
private:
	Config &config;
	enum way {lookIn,lookOut,lookAll};
	bool pass(SiftMatrix &matrix,NodeV &optimOrder,enum way way);
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Sifter_h
