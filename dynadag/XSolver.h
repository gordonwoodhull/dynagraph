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

#ifndef XSolver_h
#define XSolver_h

namespace Dynagraph {
namespace DynaDAG {

struct XSolver : XConstraintOwner {
	XSolver(Config &config, double xRes) :
		xScale(1.0/xRes),config(config) {}
	virtual ~XSolver() {} 
	const double xScale;
	void Place(DDChangeQueue &changeQ);
	void RemoveEdgeConstraints(DDModel::Edge *e);
	// XConstraintOwner
	void RemoveNodeConstraints(DDModel::Node *n);
	void InvalidateChainConstraints(DDChain *path);
	void DeleteLRConstraint(DDModel::Node *u, DDModel::Node *v);
private:
	Config &config;
	ConstraintGraph cg;
	void fixSeparation(DDModel::Node *mn);
	void doNodesep(DynaDAGLayout *subLayout);
	void doEdgesep(DynaDAGLayout *subLayout);
	void restoreNodesep(DDChangeQueue &changeQ);
	void fixEdgeCost(DDModel::Edge *me);
	void fixLostEdges(DynaDAGLayout *subLayout);
	void doEdgeCost(DynaDAGLayout *subLayout);
	void restoreEdgeCost(DDChangeQueue &changeQ);
	void stabilizeNodes(DDChangeQueue &changeQ);
	void readoutCoords();
	void checkLRConstraints();
	void checkEdgeConstraints();
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // XSolver_h
