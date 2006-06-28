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

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

Crossings calculateCrossings(Config &config) {
	Crossings cc;
	for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
		Rank *r = *ri;
		for(NodeV::iterator ni1 = r->order.begin(); ni1!=r->order.end(); ++ni1)
			for(NodeV::iterator ni2 = ni1+1; ni2!=r->order.end(); ++ni2)
				cc += uvcross(*ni1,*ni2,false,true);
	}
	return cc;
}
pair<int,Coord> calculateTotalEdgeLength(Config &config) {
	int count=0;
	Coord d(0,0);
	for(DynaDAGLayout::graphedge_iter ei = config.current->edges().begin(); ei!=config.current->edges().end(); ++ei) {
		++count;
		for(DDPath::edge_iter mei = DDp(*ei)->eBegin(); mei!=DDp(*ei)->eEnd(); ++mei) {
			DDModel::Edge *e = *mei;
			Coord d2 = gd<DDNode>(e->tail).cur-gd<DDNode>(e->head).cur;
			d += d2.Abs();
		}
	}
	return make_pair(count,d);
}

} // namespace DynaDAG
} // namespace Dynagraph

