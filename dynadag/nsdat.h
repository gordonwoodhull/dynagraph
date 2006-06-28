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

#ifndef nsdat_h
#define nsdat_h

namespace Dynagraph {
namespace NS {

enum {INEDGE,OUTEDGE};

template<typename N, typename E>
struct NSNode {
	int rank;
	int low,lim;
	int priority;
	E paredge;
	E tree[2];
	bool mark,dmark,onstack;
	bool brandNew;
	NSNode() : brandNew(true) {
		reset();
	}
	void reset() {
		rank = low = lim = priority = 0;
		paredge = tree[INEDGE] = tree[OUTEDGE] = 0;
		mark = dmark = onstack = false;
	}
};
template<typename N, typename E>
struct NSEdge {
	int cutval;
	int weight;
	int minlen;
	E prv[2],nxt[2];
	bool treeflag;
	bool brandNew;
	NSEdge() : weight(1),minlen(1),brandNew(true) {
		reset();
	}
	void reset() {
		cutval = 0;
		treeflag = false;
		prv[INEDGE] = prv[OUTEDGE] = nxt[INEDGE] = nxt[OUTEDGE] = 0;
	}
};
template<typename N, typename E>
struct NSData {
	int n_tree_edges;
	int maxiter;
	N finger;
	bool brandNew;
	NSData() : maxiter(INT_MAX),brandNew(true) {
		reset();
	}
	void reset() {
		n_tree_edges = 0;
		finger = 0;
	}
};

} // namespace NS
} // namespace Dynagraph

#endif // nsdat_h
