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

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

bool DynaDAGServer::edgeNeedsRedraw(DDPath *path,DDChangeQueue &changeQ) {
	if(path->unclippedPath.Empty()) // unclippedPath is the internal marker
		return true;
	DynaDAGLayout::Node *tailMod = changeQ.modN.find(path->layoutE->tail),
		*headMod = changeQ.modN.find(path->layoutE->head);
	// check for endpoint resize or move
	if(tailMod && igd<Update>(tailMod).flags & (DG_UPD_REGION|DG_UPD_MOVE) ||
		headMod && igd<Update>(headMod).flags & (DG_UPD_REGION|DG_UPD_MOVE))
		return true;
	if(!path->first) // flat or self edge
		return false;
	if(DynaDAGLayout::Edge *eMod = changeQ.modE.find(path->layoutE))
		if(igd<Update>(eMod).flags & DG_UPD_SUPPRESSION)
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
void DynaDAGServer::findDirtyEdges(DDChangeQueue &changeQ,bool force) {
	DynaDAGLayout::graphedge_iter ei;
	for(ei = world_->current_.edges().begin(); ei!=world_->current_.edges().end(); ++ei) {
		dgassert(!changeQ.delE.find(*ei));
		Line before = gd<EdgeGeom>(*ei).pos;
		if(force || gd<EdgeGeom>(*ei).pos.Empty() || edgeNeedsRedraw(DDp(*ei),changeQ))
			DDp(*ei)->unclippedPath.Clear();
	}
}
void DynaDAGServer::sketchEdge(DDPath *path) {
	// draw an edge just based on vnodes
	path->unclippedPath.Clear();
	EdgeGeom &eg = gd<EdgeGeom>(path->layoutE);
	path->unclippedPath.degree = 1;
	DynaDAGLayout::Node *head = path->layoutE->head,
		*tail=path->layoutE->tail;
	dgassert(head!=tail); // self-edges handled in redrawEdges
	// if a backedge (head is lower rank than tail), path->first->tail is head
	// so we have to clip accordingly and then reverse the result (for arrowheads etc.)
	bool isReversed = getEdgeDirection(path->layoutE)==reversed; //gd<DDNode>(DDp(head)->top()).rank<gd<DDNode>(DDp(tail)->bottom()).rank;
	if(isReversed)
		swap(head,tail);
	if(!path->first) {
		// there are three possible reasons why there's no path.
		// redrawEdges handles self-edges and second edges of 2-cycles
		// so we assume it's
		path->unclippedPath.push_back(DDp(tail)->pos()+eg.tailPort.pos);
		path->unclippedPath.push_back(DDp(head)->pos()+eg.headPort.pos);
	}
	else {
		path->unclippedPath.push_back(gd<DDNode>(path->first->tail).cur+eg.tailPort.pos);
		for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni)
			path->unclippedPath.push_back(gd<DDNode>(*ni).cur);
		path->unclippedPath.push_back(gd<DDNode>(path->last->head).cur+eg.headPort.pos);
	}
	bool clipFirst = eg.headClipped,
		clipLast = eg.tailClipped;
	if(isReversed)
		swap(clipFirst,clipLast);
	eg.pos.ClipEndpoints(path->unclippedPath,
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
void DynaDAGServer::drawStraightEdge(DDPath *path) {
	DynaDAGLayout::Edge *e = path->layoutE;
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
void DynaDAGServer::drawEdgeSimply(DDPath *path) {
	DynaDAGLayout::Edge *e = path->layoutE;
	if(gd<Suppression>(e).suppression==Suppression::suppressed)
		gd<EdgeGeom>(e).pos.Clear();
	else if(e->tail==e->head)
		drawSelfEdge(e);
	else
		drawStraightEdge(path);
	path->unclippedPath.Clear(); // this is not a valid drawing!
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
void clearAllEdges(DynaDAGLayout::Node *n) {
	for(DynaDAGLayout::nodeedge_iter ei = n->alledges().begin(); ei!=n->alledges().end(); ++ei)
		DDp(*ei)->unclippedPath.Clear();
}
void findFlowSlope(DynaDAGLayout::Node *cn) {
	DDMultiNode *mn = DDp(cn);
	if(!gd<NodeGeom>(cn).flow) {
		if(assign(mn->flowSlope,Coord(0,0)))
			clearAllEdges(cn);
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
	if(assign(mn->flowSlope,(avgIn+avgOut)/2*gd<NodeGeom>(cn).flow))
		clearAllEdges(cn);
}
void DynaDAGServer::findFlowSlopes(DDChangeQueue &changeQ) {
	for_each(world_->current_.nodes().begin(),world_->current_.nodes().end(),findFlowSlope);
}
void DynaDAGServer::redrawEdges(DDChangeQueue &changeQ) {
	//ObstacleAvoiderSpliner<DynaDAGLayout> obav(&world_->current_);
	for(DynaDAGLayout::graphedge_iter ei = world_->current_.edges().begin(); ei!=world_->current_.edges().end(); ++ei) {
		DynaDAGLayout::Edge *e = *ei;
		Line before = gd<EdgeGeom>(e).pos;
		if(gd<NSRankerEdge>(e).secondOfTwo)
			continue; // handled below
		dgassert(!changeQ.delE.find(e));
		if(DDp(e)->unclippedPath.Empty()) {
			if(e->tail==e->head)  // self-edge
				drawSelfEdge(e);
			else if(gd<GraphGeom>(&world_->current_).splineLevel==DG_SPLINELEVEL_VNODE ||
					!spliner.MakeEdgeSpline(DDp(e),gd<GraphGeom>(&world_->current_).splineLevel /*,obav */ ))
				sketchEdge(DDp(e));

			if(before!=gd<EdgeGeom>(e).pos)
				ModifyEdge(changeQ,e,DG_UPD_MOVE);
		}
	}
	drawSecondEdges(changeQ);
}

} // namespace DynaDAG
} // namespace Dynagraph
