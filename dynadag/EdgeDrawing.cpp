/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/

#include "DynaDAG.h"
#include <boost/bind.hpp>

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

void DynaDAGServer::sketchEdge(DynaDAGLayout::Edge *e) {
	// draw an edge just based on vnodes
	DDPath *path = DDp(e);
	Line &uncl = path->unclippedPath;
	uncl.Clear();
	EdgeGeom &eg = gd<EdgeGeom>(e);
	uncl.degree = 1;
	DynaDAGLayout::Node *head = e->head,
		*tail=e->tail;
	dgassert(head!=tail); // self-edges handled in redrawEdges
	// if a backedge (head is lower rank than tail), path->first->tail is head
	// so we have to clip accordingly and then reverse the result (for arrowheads etc.)
	bool isReversed = getEdgeDirection(e)==reversed; //gd<DDNode>(DDp(head)->top()).rank<gd<DDNode>(DDp(tail)->bottom()).rank;
	if(isReversed)
		swap(head,tail);
	if(!path->first) {
		// there are three possible reasons why there's no path.
		// redrawEdges handles self-edges and second edges of 2-cycles
		// so we assume it's
		uncl.push_back(DDp(tail)->pos()+eg.tailPort.pos);
		uncl.push_back(DDp(head)->pos()+eg.headPort.pos);
	}
	else {
		uncl.push_back(gd<DDNode>(path->first->tail).cur+eg.tailPort.pos);
		for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni)
			uncl.push_back(gd<DDNode>(*ni).cur);
		uncl.push_back(gd<DDNode>(path->last->head).cur+eg.headPort.pos);
	}
	bool clipFirst = eg.headClipped,
		clipLast = eg.tailClipped;
	if(isReversed)
		swap(clipFirst,clipLast);
	eg.pos.ClipEndpoints(uncl,
		gd<NodeGeom>(tail).pos,clipFirst?&gd<NodeGeom>(tail).region:0,
		gd<NodeGeom>(head).pos,clipLast?&gd<NodeGeom>(head).region:0);
	if(isReversed)
		reverse(eg.pos.begin(),eg.pos.end());
}
void DynaDAGServer::drawSelfEdge(DynaDAGLayout::Edge *e) {
	EdgeGeom &eg = gd<EdgeGeom>(e);
	const NodeGeom &tg = gd<NodeGeom>(e->tail),
		&hg = gd<NodeGeom>(e->head);
	Line &unclipped = DDp(e)->unclippedPath;
	unclipped.Clear();
	Coord sep = gd<GraphGeom>(e->g).separation;
	unclipped.degree = 3;
	DDModel::Node *tl = DDp(e->tail)->bottom();
	Coord tailpt = gd<EdgeGeom>(e).tailPort.pos + gd<DDNode>(tl).multi->pos();
	Coord right = Coord(tailpt.x+config.RightExtent(tl),tailpt.y),
		left = right - Coord(1.5*sep.x,0.0);
#ifndef DOWN_GREATER
	Coord dy(0.0,-sep.y/2.0);
#else
	Coord dy(0.0,sep.y/2.0);
#endif
	unclipped.push_back(left);
	unclipped.push_back(left+dy);
	unclipped.push_back(right+dy);
	unclipped.push_back(right);
	unclipped.push_back(right-dy);
	unclipped.push_back(left-dy);
	unclipped.push_back(left);
	eg.pos.ClipEndpoints(unclipped,tg.pos,eg.tailClipped?&tg.region:0,
		hg.pos,eg.headClipped?&hg.region:0);
}
void DynaDAGServer::drawStraightEdge(DynaDAGLayout::Edge *e) {
	DDPath *path = DDp(e);
	EdgeGeom &eg = gd<EdgeGeom>(e);
	Line &uncl = path->unclippedPath;
	uncl.Clear();
	const NodeGeom &tg = gd<NodeGeom>(e->tail),
		&hg = gd<NodeGeom>(e->head);
	bool clipTail = eg.tailClipped, clipHead = eg.headClipped;
	uncl.degree = 3;
	Coord tp = checkPos(gd<Suppression>(e).suppression==Suppression::tailSuppressed ? cutPos(path) : tg.pos),
		hp = checkPos(gd<Suppression>(e).suppression==Suppression::headSuppressed ? cutPos(path) : hg.pos);
	uncl.push_back(tg.pos);
	uncl.push_back((2.*tp+hp)/3.);
	uncl.push_back((tp+2.*hp)/3.);
	uncl.push_back(hg.pos);
	eg.pos.ClipEndpoints(uncl,tp,clipTail?&tg.region:0,hp,clipHead?&hg.region:0);
}
void DynaDAGServer::drawEdgeSimply(DynaDAGLayout::Edge *e) {
	if(gd<Suppression>(e).suppression==Suppression::suppressed)
		gd<EdgeGeom>(e).pos.Clear();
	else if(e->tail==e->head)
		drawSelfEdge(e);
	else
		drawStraightEdge(e);
}
void DynaDAGServer::drawSecondEdges(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::graphedge_iter ei = world_->current_.edges().begin(); ei!=world_->current_.edges().end(); ++ei)
		if(gd<NSRankerEdge>(*ei).secondOfTwo) {
			Line before = gd<EdgeGeom>(*ei).pos;
			Line &otherSide = gd<EdgeGeom>(world_->current_.find_edge((*ei)->head,(*ei)->tail)).pos;
			gd<EdgeGeom>(*ei).pos.assign(otherSide.rbegin(),otherSide.rend());
			gd<EdgeGeom>(*ei).pos.degree = otherSide.degree;
			if(before!=gd<EdgeGeom>(*ei).pos)
				ModifyEdge(changeQ,*ei,DG_UPD_MOVE);
		}
}

void findFlowSlope(DDChangeQueue &Q,DynaDAGLayout::Node *cn) {
	DDMultiNode *mn = DDp(cn);
	if(!gd<NodeGeom>(cn).flow) {
		if(assign_unclose(gd<NodeGeom>(cn).flowTan,Coord(0,0)))
			ModifyNode(Q,cn,DG_UPD_FLOWTANGENT);
		return;
	}
	Coord avgIn(0,0),avgOut(0,0);
	int nIns=0,nOuts=0;
	for(DDModel::inedge_iter ei = mn->top()->ins().begin(); ei!=mn->top()->ins().end(); ++ei) {
		Coord vec = (gd<DDNode>((*ei)->head).cur-gd<DDNode>((*ei)->tail).cur).Norm();
		if(getEdgeDirection(gd<DDEdge>(*ei).path->layoutE)==forward)
			++nIns, avgIn += vec;
		else
			++nOuts, avgOut -= vec;
	}
	for(DDModel::outedge_iter ei = mn->bottom()->outs().begin(); ei!=mn->bottom()->outs().end(); ++ei) {
		Coord vec = (gd<DDNode>((*ei)->head).cur-gd<DDNode>((*ei)->tail).cur).Norm();
		if(getEdgeDirection(gd<DDEdge>(*ei).path->layoutE)==forward)
			++nOuts, avgOut += vec;
		else
			++nIns, avgIn -= vec;
	}
	// special case flat edges (they don't have model edges)
	for(DynaDAGLayout::inedge_iter ei = cn->ins().begin(); ei!=cn->ins().end(); ++ei)
		if(getEdgeDirection(*ei)==flat)
			++nIns, avgIn += (DDp((*ei)->head)->pos() - DDp((*ei)->tail)->pos()).Norm();
	for(DynaDAGLayout::outedge_iter ei = cn->outs().begin(); ei!=cn->outs().end(); ++ei)
		if(getEdgeDirection(*ei)==flat)
			++nOuts, avgOut += (DDp((*ei)->head)->pos() - DDp((*ei)->tail)->pos()).Norm();
	if(nIns)
		avgIn /= nIns;
	if(nOuts)
		avgOut /= nOuts;
	if(assign_unclose(gd<NodeGeom>(cn).flowTan,(avgIn+avgOut)/2*gd<NodeGeom>(cn).flow))
		ModifyNode(Q,cn,DG_UPD_FLOWTANGENT);
}
void DynaDAGServer::findFlowSlopes(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::node_iter ni = world_->current_.nodes().begin(); ni!=world_->current_.nodes().end(); ++ni)
		findFlowSlope(changeQ,*ni);
	// They Say I should use for_each; so how come it never compiles?
	//for_each(world_->current_.nodes().begin(),world_->current_.nodes().end(),boost::bind(findFlowSlope,changeQ,_1));
	//for_each(world_->current_.nodes().begin(),world_->current_.nodes().end(),bind1st(findFlowSlope,changeQ));
}
bool DynaDAGServer::edgeNeedsRedraw(DynaDAGLayout::Edge *e,DDChangeQueue &changeQ) {
	DDPath *path = DDp(e);
	dgassert(!changeQ.delE.find(e));
	if(path->unclippedPath.Empty()) // unclippedPath is the internal marker
		return true;
	if(gd<EdgeGeom>(e).pos.Empty()) 
		return true;
	if(changeQ.insE.find(e))
		return true;
	if(DynaDAGLayout::Edge *eMod = changeQ.modE.find(e))
		if(igd<Update>(eMod).flags & (DG_UPD_SUPPRESSION|DG_UPD_MOVE))
			return true;
	DynaDAGLayout::Node *tailMod = changeQ.modN.find(e->tail),
		*headMod = changeQ.modN.find(e->head);
	// check for endpoint resize or move
	if(tailMod && igd<Update>(tailMod).flags & (DG_UPD_REGION|DG_UPD_MOVE|DG_UPD_FLOWTANGENT) ||
		headMod && igd<Update>(headMod).flags & (DG_UPD_REGION|DG_UPD_MOVE|DG_UPD_FLOWTANGENT))
		return true;
	for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni) {
		DDModel::Node *n = *ni;
		if(!gd<DDNode>(n).actualXValid)
			return true;
		double x = gd<DDNode>(n).actualX;
		if(DDModel::Node *left = config.Left(n)) {
			if(gd<DDNode>(left).amEdgePart()) {
				if(gd<DDNode>(left).actualXValid && gd<DDNode>(left).actualX + config.RightExtent(left) + config.RightSep(left) > x)
					return true;
			}
			else {
				if(gd<DDNode>(left).cur.x + config.RightExtent(left) + config.RightExtent(left) + config.RightSep(left) > x)
					return true;
			}
		}
		if(DDModel::Node *right = config.Right(n)) {
			if(gd<DDNode>(right).amEdgePart()) {
				if(gd<DDNode>(right).actualXValid && gd<DDNode>(right).actualX - config.LeftExtent(right) - config.LeftSep(right) < x)
					return true;
			}
			else {
				if(gd<DDNode>(right).cur.x - config.LeftExtent(right) - config.LeftSep(right) < x)
					return true;
			}
		}
	}
	return false;
}
void DynaDAGServer::redrawEdges(DDChangeQueue &changeQ,bool force) {
	//ObstacleAvoiderSpliner<DynaDAGLayout> obav(&world_->current_);
	for(DynaDAGLayout::graphedge_iter ei = world_->current_.edges().begin(); ei!=world_->current_.edges().end(); ++ei) {
		DynaDAGLayout::Edge *e = *ei;
		Line before = gd<EdgeGeom>(e).pos;
		if(gd<NSRankerEdge>(e).secondOfTwo)
			continue; // handled below
		dgassert(!changeQ.delE.find(e));
		if(force || edgeNeedsRedraw(e,changeQ)) {
			if(e->tail==e->head)  // self-edge
				drawSelfEdge(e);
			else if(gd<GraphGeom>(&world_->current_).splineLevel==DG_SPLINELEVEL_VNODE ||
					!spliner.MakeEdgeSpline(DDp(e),gd<GraphGeom>(&world_->current_).splineLevel /*,obav */ ))
				sketchEdge(e);

			if(before!=gd<EdgeGeom>(e).pos)
				ModifyEdge(changeQ,e,DG_UPD_MOVE);
		}
	}
	drawSecondEdges(changeQ);
}

} // namespace DynaDAG
} // namespace Dynagraph
