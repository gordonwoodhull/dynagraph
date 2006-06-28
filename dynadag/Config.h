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

#ifndef Config_h
#define Config_h

#include "DynaDAGServices.h"

#include "FlexiRanks.h"

namespace Dynagraph {
namespace DynaDAG {

// config needs to erase x ordering constraints but otherwise doesn't "do" constraints.
// thus XSolver should handle it.
struct XConstraintOwner {
	virtual ~XConstraintOwner() {}
	virtual void RemoveNodeConstraints(DDModel::Node *n) = 0;
	//virtual void InvalidatePathConstraints(DDPath *path) = 0;
	virtual void DeleteLRConstraint(DDModel::Node *u, DDModel::Node *v) = 0;
};

struct XGenerator {
	virtual ~XGenerator() {}
	virtual double xval(double y) = 0;
};

struct Config {
	typedef FlexiRanks Ranks;
	Config(DynaDAGServices *dynaDAG,DDModel &model,
	       DynaDAGLayout *whole,DynaDAGLayout *current,
	       XConstraintOwner *xconOwner) :
	  ranking(whole),
	  prevLow(INT_MAX),
	  model(model),
	  whole(whole),
	  current(current),
	  dynaDAG(dynaDAG),
	  xconOwner(xconOwner)
  {}
	// called by DynaDAGServer
	void Update(DDChangeQueue &changeQ);
	void SetYs();
	// services
	bool IsSuppressed(DDModel::Node *n);
	bool IsSuppressed(DDModel::Edge *e);
	double LeftExtent(DDModel::Node *n);
	double RightExtent(DDModel::Node *n);
	double TopExtent(DDModel::Node *n);
	double BottomExtent(DDModel::Node *n);
	double LeftSep(DDModel::Node *n);
	double RightSep(DDModel::Node *n);
	double UVSep(DDModel::Node *left,DDModel::Node *right);
	double CoordBetween(DDModel::Node *L, DDModel::Node *R);
	DDModel::Node *RelNode(DDModel::Node *n, int offset);
	DDModel::Node *Right(DDModel::Node *n);
	DDModel::Node *Left(DDModel::Node *n);
	void InstallAtRight(DDModel::Node *n, Ranks::index r);
	void InstallAtOrder(DDModel::Node *n, Ranks::index r, unsigned order,double x);
	void InstallAtOrder(DDModel::Node *n, Ranks::index r, unsigned order);
	void InstallAtPos(DDModel::Node *n, Ranks::index r, double x);
	void RemoveNode(DDModel::Node *n);
	void Exchange(DDModel::Node *u, DDModel::Node *v);
	void Restore(Ranks &backup);

	Ranks ranking;
	Ranks::index prevLow;
	DDModel &model;
	DynaDAGLayout *whole,*current; // same as DynaDAGServer::
private:
	DynaDAGServices *dynaDAG;
	XConstraintOwner *xconOwner;

	// update
	void makeRankList(DDChangeQueue &changeQ);
	void insertNode(DynaDAGLayout::Node *vn);
	void insertNewNodes(DDChangeQueue &changeQ);
	void buildChain(DDChain *chain, DDModel::Node *t, DDModel::Node *h, XGenerator *xgen,DynaDAGLayout::Node *vn,DynaDAGLayout::Edge *ve);
	void routeEdge(DynaDAGLayout::Edge *ve, XGenerator *xgen);
	void userRouteEdge(DDPath *path);
	void autoRouteEdge(DDPath *path);
	void adjustChain(DDChain *path,bool tail,Ranks::index dest,DynaDAGLayout::Node *vn,DynaDAGLayout::Edge *ve);
	//void adjustTail(DDChain *path, Ranks::index dest);
	void rerouteChain(DDChain *chain,int tailRank,int headRank,XGenerator *xgen);
	void autoAdjustChain(DDChain *chain,int otr,int ohr,int ntr,int nhr,DynaDAGLayout::Node *vn,DynaDAGLayout::Edge *ve);
	void autoAdjustEdge(DDPath *path);
	void insertEdge(DynaDAGLayout::Edge *ve);
	void unfixOldSingletons(DDChangeQueue &changeQ);
	void insertNewEdges(DDChangeQueue &changeQ);
	void percolate(DDModel::Node *n,DDModel::Node *ref,Ranks::index destrank);
	double placeAndReopt(DDModel::Node *n, Ranks::index r, double x);
	void moveOldNodes(DDChangeQueue &changeQ);
	void moveOldEdges(DDChangeQueue &changeQ);
	void splitRank(DDChain *chain,DDModel::Edge *e,DynaDAGLayout::Node *vn, DynaDAGLayout::Edge *ve);
	void joinRanks(DDChain *chain,DDModel::Node *n,DynaDAGLayout::Edge *ve);
	void updateRanks(DDChangeQueue &changeQ);
	void reoptAllEdgesTouched(DDChangeQueue &changeQ);
        // set Ys
	void resetRankBox(Rank *rank);
	void resetBaselines();
	// check
	void checkEdges(bool strict);
	void checkX();
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Config_h
