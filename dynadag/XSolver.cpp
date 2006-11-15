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

// changing incrementally is buggy.  not so expensive to rewrite all constraints.
#define REDO_ALL

namespace Dynagraph {
namespace DynaDAG {

// XConstraintOwner (Config & DynaDAG call-ins)
void XSolver::RemoveNodeConstraints(DDModel::Node *n) {
	cg.RemoveNodeConstraints(gd<DDNode>(n).getXcon());
	/*
	if(DDMultiNode *multi = gd<DDNode>(n).multi)
		if(multi->xcon.n) // kill all connected edge constraints (but only once)
			for(DDMultiNode::node_iter ni = multi->nBegin(); ni!=multi->nEnd(); ++ni)
				for(DDModel::nodeedge_iter ei(*ni); ei!=DDModel::nodeedge_iter(); ++ei)
					RemoveEdgeConstraints(*ei);
	*/
}
void XSolver::RemoveEdgeConstraints(DDModel::Edge *e) {
	DDCGraph::Node *cn = gd<DDEdge>(e).cn;
	if(cn) {
		cg.erase(cn);
		gd<DDEdge>(e).cn = 0;
	}
}
void XSolver::InvalidateChainConstraints(DDChain *path) {
	for(DDPath::edge_iter ei = path->eBegin(); ei!=path->eEnd(); ++ei)
		RemoveEdgeConstraints(*ei);
	for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni)
		RemoveNodeConstraints(*ni);
}
void XSolver::DeleteLRConstraint(DDModel::Node *u,DDModel::Node *v) {
	DDCGraph::Node *cn_u = u?gd<DDNode>(u).getXcon().n:cg.anchor,
		*cn_v = gd<DDNode>(v).getXcon().n;
	DDCGraph::Edge *ce;
	if(cn_u && cn_v && (ce = cg.find_edge(cn_u,cn_v)))
		cg.erase(ce);
}
// internals
void XSolver::fixSeparation(DDModel::Node *mn) {
	DDModel::Node *left, *right;	/* mn's left and right neighbors */

	/*
	double lnei_ext, lnei_sep, rnei_ext, rnei_sep,
		left_ext = config.LeftExtent(mn),
		left_sep = config.LeftSep(mn),
		right_ext = config.RightExtent(mn),
		right_sep = config.RightSep(mn);
		*/
	DDCGraph::Node *var = cg.GetVar(gd<DDNode>(mn).getXcon()),
		*left_var,*right_var;

	if((left = config.Left(mn))) {
		left_var = cg.GetVar(gd<DDNode>(left).getXcon());
		//lnei_ext = config.RightExtent(left);
		//lnei_sep = config.RightSep(left);
	}
	else {
		left_var = cg.anchor;
		/* WHATTUX?
		lnei_ext = -DDNS::NSd(left_var).rank;
		rnei_sep = 0.;
		*/
		//lnei_ext = lnei_sep = 0.;
	}

	DDCGraph::Edge *ce = cg.create_edge(left_var,var).first;
	int scaled_len = ROUND(config.UVSep(left,mn)//(lnei_ext + lnei_sep + left_sep + left_ext)
		/gd<GraphGeom>(config.current).resolution.x);
	DDNS::NSd(ce).minlen = scaled_len;
	DDNS::NSd(ce).weight = COMPACTION_STRENGTH;
	/*
	if(left && DDNS::NSd(var).rank - DDNS::NSd(left_var).rank < scaled_len)
		cg.inconsistent = true;
	*/

	if((right = config.Right(mn))) {
		right_var = cg.GetVar(gd<DDNode>(right).getXcon());
		//rnei_ext = config.LeftExtent(right);
		//rnei_sep = config.LeftSep(right);
		ce = cg.create_edge(var,right_var).first;
		scaled_len = ROUND(config.UVSep(mn,right)//(right_ext + right_sep + rnei_sep + rnei_ext)
			/gd<GraphGeom>(config.current).resolution.x);
		DDNS::NSd(ce).minlen = scaled_len;
		DDNS::NSd(ce).weight = COMPACTION_STRENGTH;
		/*
		if(DDNS::NSd(right_var).rank - DDNS::NSd(var).rank < scaled_len)
			cg.inconsistent = true;
		*/
	}
}

void XSolver::doNodesep(DynaDAGLayout *subLayout) {
	for(DynaDAGLayout::node_iter vni = subLayout->nodes().begin(); vni!=subLayout->nodes().end(); ++vni)
		for(DDMultiNode::node_iter mni = DDp(*vni)->nBegin(); mni!=DDp(*vni)->nEnd(); ++mni)
			fixSeparation(*mni);
}

void XSolver::doEdgesep(DynaDAGLayout *subLayout) {
	for(DynaDAGLayout::graphedge_iter ei = subLayout->edges().begin(); ei!=subLayout->edges().end(); ++ei) {
		DDPath *path = DDp(*ei);
		if(path->first)
			for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni)
				fixSeparation(*ni);
		else if((*ei)->head!=(*ei)->tail) { /* flat */
			DDModel::Node *u = DDp((*ei)->tail)->top(), // any subnode is fine
				*v = DDp((*ei)->head)->top();
			int ux = gd<DDNode>(u).order,
				vx = gd<DDNode>(v).order;
			if(abs(ux - vx) == 1) {
				if(ux > vx)
					swap(u,v);
				DDCGraph::Node *uvar = gd<DDNode>(u).getXcon().n,
					*vvar = gd<DDNode>(v).getXcon().n;
				DDCGraph::Edge *ce = cg.create_edge(uvar,vvar).first;
				double sep = config.RightExtent(u) + config.LeftExtent(v) + 3.0 * gd<GraphGeom>(config.whole).separation.x;
				DDNS::NSd(ce).minlen = ROUND(sep/gd<GraphGeom>(config.current).resolution.x);
			}
		}
		else {} /* self */
	}
}
void XSolver::restoreNodesep(DDChangeQueue &changeQ) {
	doNodesep(&changeQ.insN);
	doNodesep(&changeQ.modN);
	// hack: nodes of edges may also be changed
	doNodesep(&changeQ.insE);
	doNodesep(&changeQ.modE);

	doEdgesep(&changeQ.insE);
	doEdgesep(&changeQ.modE);

	for(DDModel::node_iter ni = config.model.dirty().nodes().begin(); ni!=config.model.dirty().nodes().end(); ++ni)
		fixSeparation(*ni);
}
void XSolver::fixEdgeCost(DDModel::Edge *me) {
	if(!gd<DDEdge>(me).cn) {
		gd<DDEdge>(me).cn = cg.create_node();
		gd<ConstraintType>(gd<DDEdge>(me).cn).why = ConstraintType::orderEdgeStraighten;
	}
	NSEdgePair ep(gd<DDEdge>(me).cn,cg.GetVar(gd<DDNode>(me->tail).getXcon()),cg.GetVar(gd<DDNode>(me->head).getXcon()));
	DDNS::NSd(ep.e[0]).weight = BEND_WEIGHT;
	DDNS::NSd(ep.e[1]).weight = BEND_WEIGHT;
	DDNS::NSd(ep.e[0]).minlen = 0;
	DDNS::NSd(ep.e[1]).minlen = 0;
}

void XSolver::doEdgeCost(DynaDAGLayout *subLayout) {
	for(DynaDAGLayout::graphedge_iter ei = subLayout->edges().begin(); ei!=subLayout->edges().end(); ++ei) {
		DDPath *path = DDp(*ei);
		for(DDPath::edge_iter ei2 = path->eBegin(); ei2!=path->eEnd(); ++ei2)
			fixEdgeCost(*ei2);
	}
}

// restore around changed nodes
void XSolver::fixLostEdges(DynaDAGLayout *subLayout) {
	for(DynaDAGLayout::node_iter ni = subLayout->nodes().begin(); ni!=subLayout->nodes().end(); ++ni) {
	  reports[dgr::xsolver] << "fixing multinode " << DDp(*ni) << endl;
		DDModel::Node *top = DDp(*ni)->top(),
			*bottom = DDp(*ni)->bottom();
		for(DDModel::inedge_iter ini = top->ins().begin(); ini!=top->ins().end(); ++ini)
			fixEdgeCost(*ini);
		for(DDModel::outedge_iter outi = bottom->outs().begin(); outi!=bottom->outs().end(); ++outi)
			fixEdgeCost(*outi);
	}
}
void XSolver::restoreEdgeCost(DDChangeQueue &changeQ) {
	doEdgeCost(&changeQ.insE);
	doEdgeCost(&changeQ.modE);
	fixLostEdges(&changeQ.insN);
	fixLostEdges(&changeQ.modN);
	fixLostEdges(&changeQ.insE);
	fixLostEdges(&changeQ.modE);
}

void XSolver::stabilizeNodes(DDChangeQueue &changeQ) {
#ifdef REDO_ALL
	for(DynaDAGLayout::node_iter ni = changeQ.current->nodes().begin(); ni!=changeQ.current->nodes().end(); ++ni)
        if(gd<NodeGeom>(*ni).pos.valid)  // DDp(*ni)->coordFixed) { dgassert(gd<NodeGeom>(*ni).pos.valid);
			cg.Stabilize(gd<DDNode>(DDp(*ni)->top()).getXcon(),ROUND(gd<NodeGeom>(*ni).pos.x/gd<GraphGeom>(config.current).resolution.x),STABILITY_FACTOR_X);
    /*
	DynaDAGLayout *modedges[2] = {&changeQ.insE,&changeQ.modE};
    // it's not necessary to unstabilize nodes that are connected to edges because the cost
    // of edge bends is much higher than that of stability
	int i;
	for(i = 0; i < 2; i++)
		for(DynaDAGLayout::node_iter ni = modedges[i]->nodes().begin(); ni!=modedges[i]->nodes().end(); ++ni)
			if(!DDp(*ni)->coordFixed)
				cg.Unstabilize(gd<DDNode>(DDp(*ni)->top()).getXcon());
    */
#else
	DynaDAGLayout *modnodes[2] = {&changeQ.insN,&changeQ.modN},
		*modedges[2] = {&changeQ.insE,&changeQ.modE};

	/* what about delE? make sure endpoint isn't deleted too. */

	int i;
	for(i = 0; i < 2; i++)
		for(DynaDAGLayout::node_iter ni = modnodes[i]->nodes().begin(); ni!=modnodes[i]->nodes().end(); ++ni)
            if(gd<NodeGeom>(*ni).pos.valid) { // DDp(*ni)->coordFixed) { dgassert(gd<NodeGeom>(*ni).pos.valid);
				double x = gd<NodeGeom>(*ni).pos.x;
				int ix = ROUND(xScale * x);
				cg.Stabilize(gd<DDNode>(DDp(*ni)->top()).getXcon(),ix,STABILITY_FACTOR_X);
			}
    /*
	for(i = 0; i < 2; i++)
		for(DynaDAGLayout::node_iter ni = modedges[i]->nodes().begin(); ni!=modedges[i]->nodes().end(); ++ni)
			if(!DDp(*ni)->coordFixed)
				cg.Unstabilize(gd<DDNode>(DDp(*ni)->top()).getXcon());
    */
#endif
}
void XSolver::readoutCoords() {
	int anchor_rank = DDNS::NSd(cg.anchor).rank;
	for(DDModel::node_iter ni = config.model.nodes().begin(); ni!=config.model.nodes().end(); ++ni)
		if(DDCGraph::Node *cn = gd<DDNode>(*ni).getXcon().n)
			gd<DDNode>(*ni).cur.x = (DDNS::NSd(cn).rank - anchor_rank)*gd<GraphGeom>(config.current).resolution.x;
}
// DynaDAG callin
void XSolver::Place(DDChangeQueue &changeQ) {
#ifdef REDO_ALL
	DDModel::graphedge_iter ei;
	DDModel::node_iter ni;
	for(ei = config.model.edges().begin(); ei!=config.model.edges().end(); ++ei)
		RemoveEdgeConstraints(*ei);
	for(ni = config.model.nodes().begin(); ni!=config.model.nodes().end(); ++ni)
		RemoveNodeConstraints(*ni);
	for(ni = config.model.nodes().begin(); ni!=config.model.nodes().end(); ++ni)
		fixSeparation(*ni);
	for(ei = config.model.edges().begin(); ei!=config.model.edges().end(); ++ei)
		fixEdgeCost(*ei);
#else
	// obliterate constraints on all changed nodes & edges
	for(DynaDAGLayout::graphedge_iter ei = changeQ.modE.edges().begin(); ei!=changeQ.modE.edges().end(); ++ei)
		if(igd<Update>(*ei)&DG_UPD_MOVE)
			InvalidateChainConstraints(DDp(*ei));
	for(DynaDAGLayout::node_iter ni = changeQ.modN.nodes().begin(); ni!=changeQ.modN.nodes().end(); ++ni)
		if(igd<Update>(*ni)&DG_UPD_MOVE)
			InvalidateChainConstraints(DDp(*ni));
	restoreNodesep(changeQ);
	restoreEdgeCost(changeQ);
#endif
	//timer.LoopPoint(dgr::timing,"XConstraints");
	config.model.dirty().clear();
	stabilizeNodes(changeQ);
	// if(cg.inconsistent || 1)
	{
		checkLRConstraints();
		checkEdgeConstraints();
		try {
			DDNS().Solve(&cg,NS::RECHECK|NS::VALIDATE|NS::NORMALIZE);
		}
		catch(DDNS::CycleException ce) {
			for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
				reports[dgr::error] << config.ranking.IndexOfIter(ri) << "> ";
				for(NodeV::iterator ni = (*ri)->order.begin(); ni!=(*ri)->order.end(); ++ni) {
					if(gd<DDNode>(*ni).amNodePart()) {
						bool found = gd<DDNode>(*ni).multi->xcon.n==ce.involving;
						reports[dgr::error] << 'n' << thing(*ni) << (found?'*':' ');
					}
					else
						reports[dgr::error] << 'e' << thing(*ni) << ' ';
				}
				reports[dgr::error] << endl;
			}
			throw BadXConstraints();
		}
		catch(DDNS::DisconnectedException) {
			reports[dgr::error] << "internal error: disconnected constraint graph!" << endl;
			throw BadXConstraints();
		}
		readoutCoords();
		// cg.inconsistent = false;
	}
}

} // namespace DynaDAG
} // namespace Dynagraph
