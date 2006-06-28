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

#ifndef Crossings_h
#define Crossings_h

namespace Dynagraph {
namespace DynaDAG {

struct Crossings {
	unsigned edgeEdgeCross,nodeEdgeCross,nodeNodeCross;
	Crossings() : edgeEdgeCross(0),nodeEdgeCross(0),nodeNodeCross(0) {}
	Crossings(DDModel::Edge *e,DDModel::Edge *f) : edgeEdgeCross(0),nodeEdgeCross(0),nodeNodeCross(0) {
		add(e,f);
	}
	void add(DDModel::Edge *e,DDModel::Edge *f) {
		switch(gd<DDEdge>(e).amNodePart()+gd<DDEdge>(f).amNodePart()) {
		case 0:
			edgeEdgeCross++;
			break;
		case 1:
			nodeEdgeCross++;
			break;
		case 2:
			nodeNodeCross++;
			break;
		}
	}
	Crossings &operator +=(const Crossings &c) {
		edgeEdgeCross += c.edgeEdgeCross;
		nodeEdgeCross += c.nodeEdgeCross;
		nodeNodeCross += c.nodeNodeCross;
		return *this;
	}
};
Crossings uvcross(DDModel::Node *v, DDModel::Node *w, bool use_in, bool use_out);
inline unsigned crossweight(Crossings cc) {
  return cc.edgeEdgeCross + NODECROSS_PENALTY*cc.nodeEdgeCross +
	  NODECROSS_PENALTY*NODECROSS_PENALTY*cc.nodeNodeCross;
}
inline unsigned crosslight(Crossings cc) {
	return cc.edgeEdgeCross + cc.nodeEdgeCross + cc.nodeNodeCross;
}

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Crossings_h
