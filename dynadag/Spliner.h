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

#ifndef Spliner_h
#define Spliner_h

namespace Dynagraph {
namespace DynaDAG {

struct Spliner {
	Spliner(Config &config) : config(config) {}
	friend struct TempRoute;
	bool MakeEdgeSpline(DDPath *path,SpliningLevel splineLevel);//,ObstacleAvoiderSpliner<DynaDAGLayout> &obav);
private:
	Config &config;
	void forwardEdgeRegion(DDModel::Node *tl, DDModel::Node *hd,DDPath *inp, Coord tp, Coord hp, Line &out);
	void flatEdgeRegion(DDModel::Node *tl, DDModel::Node *hd, Coord tp, Coord hp, Line &out);
	void adjustPath(DDPath *path);
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Spliner_h
