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

#ifndef DynaDAGModel_h
#define DynaDAGModel_h

#include "Constraints.h"

namespace Dynagraph {
namespace DynaDAG {

template<typename N,typename E>
struct Chain;
template<typename N,typename E>
struct MultiNode;
template<typename N,typename E>
struct Path;

typedef enum UpDown_ {UP,DOWN} UpDown;
typedef enum LeftRight_ {LEFT=-1,RIGHT=1} LeftRight;
struct Median {
	double val; // value
	bool exists, // if defined
		cached; // if definition is current
	Median() : val(0),exists(false),cached(false) {}
};
template<typename N,typename E>
struct DDNodeT {
	// general
	MultiNode<N,E> *multi; // the chain of nodes this is part of, if representing layout node
	NodeConstraints xcon; // constraints for x Coord
	NodeConstraints &getXcon() { // defer to multi's if any
		if(multi)
			return multi->xcon;
		else
			return xcon;
	}
	// config
	Median med[2]; // UpDown
	int rank,
		order,
		preorder; // for optimizer to have consistent indices
	bool inConfig;
	// old way of excluding nodes from crossopt
	//int orderConstraint;
	// geometry
	Position cur, prev;
	// only used in path vnodes:
	double actualX;	// spline intercept
	bool actualXValid;

	DDNodeT() : multi(0),rank(0),order(0),inConfig(false),actualX(0),
		actualXValid(false) {}
	bool amNodePart() {
		return multi!=0;
	}
	bool amEdgePart() {
		return multi==0;
	}
};
template<typename N,typename E>
struct DDEdgeT {
	Path<N,E> *path; // the chain of edges this is part of, if representing a layout edge
	DDCGraph::Node *cn; // X constraint node
	DDEdgeT() : path(0),cn(0) {}
	DDEdgeT(const DDEdgeT &other) : path(other.path),cn(0) {}
	DDEdgeT &operator=(const DDEdgeT &other) {
		path = other.path;
		cn = 0;
	}
	~DDEdgeT() {
		dgassert(!cn);
	}
	bool amNodePart() {
		return path==0;
	}
	bool amEdgePart() {
		return path!=0;
	}
};

// changing incrementally is buggy.  not so expensive to rewrite all constraints.
#define REDO_ALL_XCONSTRAINTS

struct DDModel : LGraph<ADTisCDT,Nothing,DDNodeT<void,void>,DDEdgeT<void,void>,Nothing,Nothing > {
	typedef LGraph<ADTisCDT,Nothing,DDNodeT<void,void>,DDEdgeT<void,void>,Nothing,Nothing > G;
	// the real types, hampered by circular typing problems
	typedef Chain<Node,Edge> C;
	typedef MultiNode<Node,Edge> MN;
	typedef Path<Node,Edge> P;
	typedef DDNodeT<Node,Edge> DDN;
	typedef DDEdgeT<Node,Edge> DDE;
#ifndef REDO_ALL_XCONSTRAINTS
	DDModel() { dirty_ = new G(this); }
	~DDModel() { delete dirty_; }
	G &dirty() { return *dirty_; }
private:
	G *dirty_;
#endif
};
typedef DDModel::C DDChain;
typedef DDModel::MN DDMultiNode;
typedef DDModel::P DDPath;
typedef DDModel::DDN DDNode;
typedef DDModel::DDE DDEdge;

// convolution to work around type-punning warnings
// (but maybe this is "the truth"?)
struct DDModelNodePointer {
	DDMultiNode *model;
};
struct DDModelEdgePointer {
	DDPath *model;
};
inline DDMultiNode *&DDp(DynaDAGLayout::Node *n) {
	return gd2<DDModelNodePointer,ModelPointer>(n).model;
}
inline DDPath *&DDp(DynaDAGLayout::Edge *e) {
	return gd2<DDModelEdgePointer,ModelPointer>(e).model;
}
typedef std::vector<DDModel::Node*> NodeV;
typedef std::vector<DDModel::Edge*> EdgeV;
typedef std::set<DynaDAGLayout::Node*> NodeSet;

} // namespace Dynagraph
} // namespace DynaDAG

// specialize LGraph's gd for model nodes & edges
template<>
inline Dynagraph::DynaDAG::DDNode &gd<Dynagraph::DynaDAG::DDNode,Dynagraph::DynaDAG::DDModel::Node>(Dynagraph::DynaDAG::DDModel::Node *n) {
	return gd2<Dynagraph::DynaDAG::DDNode,Dynagraph::DynaDAG::DDNodeT<void,void> >(n);
}
template<>
inline Dynagraph::DynaDAG::DDEdge &gd<Dynagraph::DynaDAG::DDEdge,Dynagraph::DynaDAG::DDModel::Edge>(Dynagraph::DynaDAG::DDModel::Edge *e) {
	return gd2<Dynagraph::DynaDAG::DDEdge,Dynagraph::DynaDAG::DDEdgeT<void,void> >(e);
}
/*
// is this necessary?
template<>
inline const Dynagraph::DynaDAG::DDNode &gd<const Dynagraph::DynaDAG::DDNode,const Dynagraph::DynaDAG::DDModel::Node>(const Dynagraph::DynaDAG::DDModel::Node *n) {
	return gd2<const Dynagraph::DynaDAG::DDNode,Dynagraph::DynaDAG::DDNodeT<void,void> >(n);
}
template<>
inline const Dynagraph::DynaDAG::DDEdge &gd<const Dynagraph::DynaDAG::DDEdge,const Dynagraph::DynaDAG::DDModel::Edge>(const Dynagraph::DynaDAG::DDModel::Edge *e) {
	return gd2<const Dynagraph::DynaDAG::DDEdge,Dynagraph::DynaDAG::DDEdgeT<void,void> >(e);
}
*/

#endif // DynaDAGModel_h
