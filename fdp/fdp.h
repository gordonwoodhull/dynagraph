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


#ifndef FDP_H
#define FDP_H

#include "common/ChangeQueue.h"
#include "common/ChangeProcessor.h"
#include "FDPLayout.h"
#include "FDPModel.h"
#include "grid.h"

namespace Dynagraph {
namespace FDP {

struct Inconsistency : DGException {
  Inconsistency() : DGException("fdp's internal model has become inconsistent with the client graph",true) {}
};
struct StillHasEdges : DGException {
  StillHasEdges() : DGException("a node was deleted without all of its edges being deleted (impossible!)",true) {}
};

struct FDPServer : LinkedChangeProcessor<FDPLayout>,Grid::Visitor {
	int numIters;
	bool useComp,
		useGrid;
	double Width, // Width of bounding box
		Height,  // Height of bounding box
		T0,  // Initial temperature
		K,K2, // Edge length, squared
		CellSize, // Cell width and height
		RepFactor,
		AttFactor,
		Afact2, // Phase 2 AttFactor
		Rfact2, // Phase 2 RepFactor
		Radius2; // Radius of interaction squared. Anything outside of the radius has no effect on node

	FDPServer(ChangingGraph<FDPLayout> *world) 
		: LinkedChangeProcessor<FDPLayout>(world),
		numIters(40),
		useComp(false),
		useGrid(true),
		Width(8.5),Height(11.0),
		T0(-1.0),
		K(1.0),K2(1.0),
		CellSize(0.0),
		RepFactor(1.0),
		AttFactor(1.0),
		Afact2(0.001),
		Rfact2(1.0),
		Radius2(0.0) {}
	~FDPServer() {}
	// ChangeProcessor
	void Process();
	// Grid::Visitor
	int VisitCell(Cell *cell,Grid *grid);
private:
	FDPModel model;

	void createModelNode(FDPLayout::Node *n);
	void createModelEdge(FDPLayout::Edge *e);
	void deleteModelNode(FDPLayout::Node *n);
	void deleteModelEdge(FDPLayout::Edge *e);

	Position findMedianSize();
	void setParams(Coord avgsize);
	double cool(int t);
	void doRep(FDPModel::Node *p, FDPModel::Node *q, double xdelta, double ydelta, double dist2);
	void applyRep(FDPModel::Node *p, FDPModel::Node *q);
	void applyAttr(FDPModel::Node *p, FDPModel::Node *q);
	void doNeighbor(Grid *grid, int i, int j, const node_list &nodes);
	int gridRepulse(Dt_t*dt, void*v, void*g);
	void adjust(FDPModel *g, double temp, Grid *grid);
	void positionNodes(FDPModel *g, Grid *grid);
	void layout();
};

} // namespace FDP
} // namespace Dynagraph

#endif // FDP_H
