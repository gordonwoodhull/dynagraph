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

#ifndef DynaDAG_h
#define DynaDAG_h

#include "DynaDAGLayout.h"
#include "common/ChangeQueue.h"
#include "common/ChangeProcessor.h"
// #include "shortspline/ObstacleAvoiderSpliner.h"
#include "ns.h"
#include <float.h>

#include "DynaDAGModel.h"
#include "DynaDAGChains.h"
#include "Medians.h"
#include "DynaDAGDefs.h"
#include "Config.h"
#include "SiftMatrix.h"
#include "ConstraintGraph.h"
#include "DotlikeOptimizer.h"
#include "XSolver.h"
#include "FlexiSpliner.h"

namespace Dynagraph {
namespace DynaDAG {

#pragma warning (disable : 4355)

void getCrossoptModelNodes(DynaDAGLayout &nodes,DynaDAGLayout &edges,NodeV &out);

struct DynaDAGServer : LinkedChangeProcessor<DynaDAGLayout>,DynaDAGServices {
	DDModel model; // whole graph + virtual nodes & edges for tall nodes & edge chains
	Config config;	// indexes layout nodes by rank and order
	Optimizer *optimizer;
	XSolver xsolver;
	FlexiSpliner spliner;

	DynaDAGServer(ChangingGraph<DynaDAGLayout> *world) 
		: LinkedChangeProcessor<DynaDAGLayout>(world),
		model(),
		config(this,model,&world->whole_,&world->current_,&xsolver),
		optimizer(new DotlikeOptimizer(config)),
		xsolver(config),
		spliner(config)
	{}
	~DynaDAGServer();
	// ChangeProcessor
	void Open() {
		NextOpen();
		this->world_->Q_.Clear();
	}
	void Process();
	// DynaDAGServices
	std::pair<DDMultiNode*,DDModel::Node*> OpenModelNode(DynaDAGLayout::Node *layoutN);
	void CloseModelNode(DDModel::Node *n);
	std::pair<DDPath*,DDModel::Edge*> OpenModelEdge(DDModel::Node *u, DDModel::Node *v, DynaDAGLayout::Edge *layoutE);
	void CloseModelEdge(DDModel::Edge *e);
	void CloseChain(DDChain *chain,bool killEndNodes);
	Optimizer *GetOptimizer();
private:
	void closeLayoutNode(DynaDAGLayout::Node *n);
	void closeLayoutEdge(DynaDAGLayout::Edge *e);
	void executeDeletions(DDChangeQueue &changeQ);
	void findOrdererSubgraph(DDChangeQueue &changeQ,DynaDAGLayout &outN,DynaDAGLayout &outE);
	void updateBounds(DDChangeQueue &changeQ);
	void moveNodesBasedOnModel(DDChangeQueue &changeQ);

	void sketchEdge(DynaDAGLayout::Edge *e); 
	void drawStraightEdge(DynaDAGLayout::Edge *e); 
	void drawSelfEdge(DynaDAGLayout::Edge *e); 
	void drawEdgeSimply(DynaDAGLayout::Edge *e); 

	void findFlowSlopes(DDChangeQueue &changeQ);
	bool edgeNeedsRedraw(DynaDAGLayout::Edge *e,DDChangeQueue &changeQ);
	void redrawEdges(DDChangeQueue &changeQ,bool force);
	void drawSecondEdges(DDChangeQueue &changeQ);
	void generateIntermediateLayout(DDChangeQueue &changeQ);
	void rememberOld();
	void dumpModel();
};

/*
	EXCEPTIONS
*/
template<typename Layout>
struct NailWithoutPos : DGException {
	typename Layout::Node *n;
	NailWithoutPos(typename Layout::Node *n) :
	  DGException("nail a node without specifying a position"),
	  n(n) {}
};
struct BackForth : DGException {
	DynaDAGLayout::Edge *e;
	BackForth(DynaDAGLayout::Edge *e) : DGException("dynadag can't handle a->b->a  (a->b->c->a is okay)"),
	  e(e) {}
};


} // namespace DynaDAG
} // namespace Dynagraph

#endif // DynaDAG_h
