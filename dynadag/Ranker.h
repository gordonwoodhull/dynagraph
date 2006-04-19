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

#ifndef Ranker_h
#define Ranker_h

namespace Dynagraph {
namespace DynaDAG {

struct Ranker {
	Ranker(DynaDAGServices *dynaDAG,Config &config) : dynaDAG(dynaDAG),config(config),top(cg.create_node()) {}
	// called by DynaDAG
	void RemoveLayoutNodeConstraints(DDMultiNode *m);
	void RemovePathConstraints(DDPath *path);
	void Rerank(DDChangeQueue &changeQ);
private:
	DynaDAGServices *dynaDAG;
	Config &config;
	ConstraintGraph cg;
	ConstraintGraph::Node *top; // to pull loose nodes upward
	void makeStrongConstraint(DDPath *path);
	void makeWeakConstraint(DDPath *path);
	void fixNode(DynaDAGLayout::Node *n,bool fix);
	void doNodeHeight(DynaDAGLayout::Node *n);
	void moveOldNodes(DDChangeQueue &changeQ);
	void insertNewNodes(DDChangeQueue &changeQ);
	void stabilizePositionedNodes(DDChangeQueue &changeQ);
	void insertNewEdges(DDChangeQueue &changeQ);
	bool simpleCase(DDChangeQueue &changeQ);
	void recomputeRanks(DDChangeQueue &changeQ);
#ifdef FLEXIRANKS
	void makeRankList(DDChangeQueue &changeQ);
#endif
	void findEdgeDirections();
	void checkStrongConstraints(DDChangeQueue &changeQ);
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Ranker_h
