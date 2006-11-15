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
#include "Measurements.h"
#include "common/time-o-matic.h"

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

DynaDAGServer::~DynaDAGServer() {
	for(DynaDAGLayout::graphedge_iter i = world_->current_.edges().begin();i!=world_->current_.edges().end();++i)
		closeLayoutEdge(*i);
	for(DynaDAGLayout::node_iter j(world_->current_.nodes().begin()); j!=world_->current_.nodes().end(); ++j)
		closeLayoutNode(*j);
	dgassert(model.empty());
}
// DynaDAGServices
pair<DDMultiNode*,DDModel::Node*> DynaDAGServer::OpenModelNode(DynaDAGLayout::Node *layoutN) {
	DDMultiNode *m = 0;
	DDModel::Node *mn = model.create_node();
	if(layoutN) { // part of multinode: attach multi to model & view nodes
		dgassert(layoutN->g==&world_->whole_); // don't store the wrong subnode
		m = DDp(layoutN); // see if view node's already been modeled
		if(!m) {
			(m = new DDMultiNode)->layoutN = layoutN;
			m->node = mn;
			DDp(layoutN) = m;
		}
		gd<DDNode>(mn).multi = m;
	}
	return make_pair(m,mn);
}
void DynaDAGServer::CloseModelNode(DDModel::Node *n) {
	/* must already be isolated in graph */
	dgassert(n->ins().size()==0 && n->outs().size()==0);
	xsolver.RemoveNodeConstraints(n);
	config.RemoveNode(n);
	model.erase_node(n);
}
pair<DDPath*,DDModel::Edge*> DynaDAGServer::OpenModelEdge(DDModel::Node *u, DDModel::Node *v, DynaDAGLayout::Edge *layoutE) {
	DDPath *p = 0;
	DDModel::Edge *me = 0;
	if(u&&v)
		me = model.create_edge(u,v).first;
	if(layoutE) { // part of path: attach path to model & view edges
		dgassert(layoutE->g==&world_->whole_); // don't store the wrong subedge
		p = DDp(layoutE); // see if view edge's already been modeled
		if(!p) {
			(p = new DDPath)->layoutE = layoutE;
			DDp(layoutE) = p;
		}
		if(me)
			gd<DDEdge>(me).path = p;

	}
	return make_pair(p,me);
}
void DynaDAGServer::CloseModelEdge(DDModel::Edge *e) {
	xsolver.RemoveEdgeConstraints(e);
	model.erase_edge(e);
}
void DynaDAGServer::CloseChain(DDChain *chain,bool killEndNodes) {
	if(!chain->first)
		return;
	DDModel::Edge *ei = chain->first;
	DDModel::Node *ni;
	while(1) {
		ni = ei->head;
		DDModel::Node *killn = 0;
		if(killEndNodes || ei!=chain->first)
			killn = ei->tail;
		CloseModelEdge(ei);
		if(killn)
			CloseModelNode(killn);
		if(ei==chain->last)
			break;
		dgassert(ni->outs().size()==1);
		ei = *ni->outs().begin();
	}
	if(killEndNodes)
		CloseModelNode(ni);
	chain->first = chain->last = 0;
}
Optimizer *DynaDAGServer::GetOptimizer() {
	return optimizer;
}
// private methods
void DynaDAGServer::closeLayoutNode(DynaDAGLayout::Node *n) {
	DDMultiNode *m = DDp(n);
	//xsolver.RemoveLayoutNodeConstraints(m);
	CloseChain(m,true);
	if(m->node)
		CloseModelNode(m->node);
	delete m;
	DDp(n) = 0;
}
void DynaDAGServer::closeLayoutEdge(DynaDAGLayout::Edge *e) {
	DDPath *p = DDp(e);
	//reports[dgr::bug] << "e " << e << " p " << p << endl;
	dgassert(p);
	if(p->first) {
		InvalidateMVal(p->first->tail,DOWN);
		InvalidateMVal(p->last->head,UP);
	}
	CloseChain(p,false);
	delete p;
	DDp(e) = 0;
}
void DynaDAGServer::executeDeletions(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::graphedge_iter ei = changeQ.delE.edges().begin(); ei!=changeQ.delE.edges().end();) {
		DynaDAGLayout::Edge *e = *ei++;
		closeLayoutEdge(e);
	}
	for(DynaDAGLayout::node_iter ni = changeQ.delN.nodes().begin(); ni!=changeQ.delN.nodes().end();) {
		DynaDAGLayout::Node *n = *ni++;
		closeLayoutNode(n);
	}
}
// pretty aggressive (not ENOUGH)
void DynaDAGServer::findOrdererSubgraph(DDChangeQueue &changeQ,DynaDAGLayout &outN,DynaDAGLayout &outE) {
	// do crossing optimization on all inserted nodes & edges...
	outN = changeQ.insN;
	outE = changeQ.insE;
	// all moved...
	DynaDAGLayout::node_iter ni;
	for(ni = changeQ.modN.nodes().begin(); ni!=changeQ.modN.nodes().end(); ++ni)
		if(igd<Update>(*ni).flags&DG_UPD_MOVE)
			outN.insert(*ni);
	for(DynaDAGLayout::graphedge_iter ei=changeQ.modE.edges().begin(); ei!=changeQ.modE.edges().end(); ++ei)
		if((igd<Update>(*ei).flags&DG_UPD_MOVE) && !userDefinedMove(*ei))
			outE.insert(*ei);
	// and all adjacent (this will add the edges off of a node that has a new or changed edge, but not the other ends of those edges)
	outN |= outE; // nodes adjacent to edges
	// note that code below is iterating on the wrong subgraph which might be why this didn't work that well
	for(ni = outN.nodes().begin(); ni!=outN.nodes().end(); ++ni) // edges adjacent to nodes
		for(DynaDAGLayout::nodeedge_iter ei = (*ni)->alledges().begin(); ei!=(*ni)->alledges().end(); ++ei)
			outE.insert(*ei);
	if(reports.enabled(dgr::dynadag)) {
		loops.Field(dgr::dynadag,"number of layout nodes",world_->current_.nodes().size());
		loops.Field(dgr::dynadag,"layout nodes for crossopt",outN.nodes().size());
		loops.Field(dgr::dynadag,"layout edges for crossopt",outE.edges().size());
	}
}
void DynaDAGServer::updateBounds(DDChangeQueue &changeQ) {
	bool got = false;
	double glb=0.0,grb=0.0;  // init for gcc's sake argh
	for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri)
		if((*ri)->order.size()) {
			DDModel::Node *left = (*ri)->order.front();
			double lb = gd<DDNode>(left).cur.x - config.LeftExtent(left);
			if(!got || glb > lb)
				glb = lb;
			DDModel::Node *right = (*ri)->order.back();
			double rb = gd<DDNode>(right).cur.x + config.RightExtent(right);
			if(!got || grb < rb)
				grb = rb;
			got = true;
		}
		Bounds bb;
		if(got) {
			bb.valid = true;
			bb.l = glb;
			bb.t = config.ranking.front()->yAbove(0);
			bb.r = grb;
			bb.b = config.ranking.back()->yBelow(0);
		}
		if(gd<GraphGeom>(&world_->current_).bounds != bb) {
			gd<GraphGeom>(&world_->current_).bounds = bb;
			ModifyFlags(changeQ) |= DG_UPD_BOUNDS;
		}
}
void DynaDAGServer::findChangedNodes(DDChangeQueue &changeQ) {
	// calculate how much nodes moved
	Coord moved(0,0);
	int nmoves=0;
	for(DynaDAGLayout::node_iter ni = world_->current_.nodes().begin(); ni!=world_->current_.nodes().end(); ++ni) {
		if(!changeQ.delN.find(*ni)) {
			Position pos = DDp(*ni)->pos();
			dgassert(pos.valid);
			Position &p = gd<NodeGeom>(*ni).pos;
			if(!is_vclose(p,pos)) {
				if(p.valid) {
					moved += (p-pos).Abs();
					++nmoves;
				}
				p = pos;
				ModifyNode(changeQ,*ni,DG_UPD_MOVE);
			}
		}
	}
	loops.Field(dgr::stability,"layout nodes moved",nmoves);
	loops.Field(dgr::stability,"node x movement",moved.x);
	loops.Field(dgr::stability,"node y movement",moved.y);
}
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
	uncl.degree = 3;
	const Coord &tp = tg.pos,
		&hp = hg.pos;
	uncl.push_back(tp);
	uncl.push_back((2.*tp+hp)/3.);
	uncl.push_back((tp+2.*hp)/3.);
	uncl.push_back(hp);
	eg.pos.ClipEndpoints(uncl,tg.pos,eg.tailClipped?&tg.region:0,
		hg.pos,eg.headClipped?&hg.region:0);
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
void DynaDAGServer::findDirtyEdges(DDChangeQueue &changeQ,bool force) {
	DynaDAGLayout::graphedge_iter ei;
	for(ei = world_->current_.edges().begin(); ei!=world_->current_.edges().end(); ++ei) {
		dgassert(!changeQ.delE.find(*ei));
		Line before = gd<EdgeGeom>(*ei).pos;
		if(force || gd<EdgeGeom>(*ei).pos.Empty() || edgeNeedsRedraw(DDp(*ei),changeQ))
			DDp(*ei)->unclippedPath.Clear();
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
void DynaDAGServer::generateIntermediateLayout(DDChangeQueue &changeQ) {
	findChangedNodes(changeQ);
	for(DynaDAGLayout::graphedge_iter ei = changeQ.insE.edges().begin(); ei!=changeQ.insE.edges().end(); ++ei)
		drawEdgeSimply(DDp(*ei));
	for(DynaDAGLayout::graphedge_iter ei = changeQ.modE.edges().begin(); ei!=changeQ.modE.edges().end(); ++ei)
		if(igd<Update>(*ei).flags & DG_UPD_MOVE)
			drawEdgeSimply(DDp(*ei));
}
void DynaDAGServer::rememberOld() { // dd_postprocess
	for(DDModel::node_iter ni = model.nodes().begin(); ni!=model.nodes().end(); ++ni) {
		DDNode &ddn = gd<DDNode>(*ni);
		ddn.prev = ddn.cur;
		// ddn.orderFixed = true;
	}
	for(DynaDAGLayout::node_iter vni = world_->current_.nodes().begin(); vni!=world_->current_.nodes().end(); ++vni) {
		DDMultiNode *n = DDp(*vni);
		if(!n)
			continue; // deleted
		//n->coordFixed = true;
		gd<NSRankerNode>(*vni).oldTopRank = gd<DDNode>(n->top()).rank;
		gd<NSRankerNode>(*vni).oldBottomRank = gd<DDNode>(n->bottom()).rank;
	}
}
void DynaDAGServer::dumpModel() {
	if(!reports.enabled(dgr::modelDump))
		return;
	reports[dgr::modelDump] << "digraph dynagraphModel {" << endl;
	for(DDModel::node_iter ni = model.nodes().begin(); ni!=model.nodes().end(); ++ni)
		reports[dgr::modelDump] << "\t\"" << *ni << "\" [label=\"" << *ni << "\\n" << gd<DDNode>(*ni).multi << "\"];" << endl;
	for(DDModel::graphedge_iter ei = model.edges().begin(); ei!=model.edges().end(); ++ei)
		reports[dgr::modelDump] << "\t\"" << (*ei)->tail << "\"->\"" << (*ei)->head << "\" [label=\"" << *ei << "\\n" << gd<DDEdge>(*ei).path << "\"];" << endl;
	reports[dgr::modelDump] << "}" << endl;
}
void ClearInsDel(DDChangeQueue &Q) {
	Q.ExecuteDeletions();
	// turn inserts into modifies (this belongs within ChangeQueue, except the DG::Update specific part)
	for(DynaDAGLayout::node_iter ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end();) {
		DynaDAGLayout::Node *n = *ni++,
			*wn = Q.whole->find(n);
		Q.insN.erase(n);
		ModifyNode(Q,wn,DG_UPD_MOVE);
	}
	for(DynaDAGLayout::graphedge_iter ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end();) {
		DynaDAGLayout::Edge *e = *ei++,
			*we = Q.whole->find(e);
		Q.insE.erase(e);
		ModifyEdge(Q,we,DG_UPD_MOVE);
	}
	Q.insE.clear(); // erase the nodes
}
void ClearStrAttrChanges(DDChangeQueue &Q) {
	igd<StrAttrChanges>(Q.ModGraph()).clear();
	for(DynaDAGLayout::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni) 
		igd<StrAttrChanges>(*ni).clear();
	for(DynaDAGLayout::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei) 
		igd<StrAttrChanges>(*ei).clear();
}
// again this seems like something any layout engine might want
bool ChangesAreRelevant(DDChangeQueue &Q) {
	if(Q.Empty())
		return igd<Update>(Q.ModGraph()).flags;
	if(!Q.insN.empty() || !Q.insE.empty() || !Q.delN.empty() || !Q.delE.empty())
		return true;
	for(DynaDAGLayout::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
		if(igd<Update>(*ni).flags)
			return true;
	for(DynaDAGLayout::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
		if(igd<Update>(*ei).flags)
			return true;
	return false;
}
// something to compare pulse times to see if a limit passed
struct PhaseMinder {
	typedef vector<DString> PhaseList;
	PhaseList phases_;
	PhaseMinder(const char *phases[],int nphases) {
		phases_.reserve(nphases);
		for(int i=0; i<nphases; ++i)
			phases_.push_back(phases[i]);
	}
	bool Check2(const StrAttrs &pulseAttrs,DString currPhase,DString currStep) {
		DString pulsePhase = pulseAttrs.look("phase");
		// if pulse/interrupt does not specify a phase, time limit has always passed
		if(!pulsePhase)
			return true;
		PhaseList::iterator pi = find(phases_.begin(),phases_.end(),pulsePhase),
			ci = find(phases_.begin(),phases_.end(),currPhase);
		// if either pulse or current phase is not in list, then limit has passed 
		if(pi==phases_.end() || ci==phases_.end())
			return true;
		// if pulse is earlier in list than current, then limit has passed
		if(pi<ci)
			return true;
		// however, if current is earlier, then limit has NOT passed
		if(pi>ci)
			return false;
		// ...phase is same...
		DString pulseStep = pulseAttrs.look("step");
		// if pulse does not specify step, then limit has passed
		if(!pulseStep)
			return true;
		// if step is same then limit has passed
		if(pulseStep==currStep)
			return true;
		// step is either "done" or an integer
		if(currStep=="done")
			return true;
		if(pulseStep=="done")
			return false;
		return atoi(pulseStep.c_str()) >= atoi(currStep.c_str());
	}
	bool Check(const StrAttrs &pulseAttrs,DString currPhase,DString currStep) {
		DString pulsePhase = pulseAttrs.look("phase");
		reports[dgr::bug] << "pulse phase " << (pulsePhase?pulsePhase:"(none)") << " curr phase " << currPhase;
		bool res = Check2(pulseAttrs,currPhase,currStep);
		reports[dgr::bug] << " result " << (res?"true":"false") << endl;
		return res;
	}
};
const char *g_dynadagPhases[] = {"update","untangle","xopt","finish"};
const int g_nDynadagPhases = sizeof(g_dynadagPhases)/sizeof(const char*);
PhaseMinder g_dynadagPhaseMinder(g_dynadagPhases,g_nDynadagPhases);

void DynaDAGServer::Process() {
	ChangeQueue<DynaDAGLayout> &Q = this->world_->Q_;
	loops.Field(dgr::dynadag,"nodes inserted - input",Q.insN.nodes().size());
	loops.Field(dgr::dynadag,"edges inserted - input",Q.insE.edges().size());
	loops.Field(dgr::dynadag,"nodes modified - input",Q.modN.nodes().size());
	loops.Field(dgr::dynadag,"edges modified - input",Q.modE.edges().size());
	loops.Field(dgr::dynadag,"nodes deleted - input",Q.delN.nodes().size());
	loops.Field(dgr::dynadag,"edges deleted - input",Q.delE.nodes().size());
	
	if(!ChangesAreRelevant(Q)) {
		NextProcess();
		Q.Clear(); 
		return;
	}

	// erase model objects for everything that's getting deleted
	executeDeletions(Q);
	timer.LoopPoint(dgr::timing,"preliminary");

	// figure out subgraph for crossing optimization
	//DynaDAGLayout crossN(&world_->current_),crossE(&world_->current_);
	//findOrdererSubgraph(Q,crossN,crossE);
	//optimizer = optChooser.choose(crossN.nodes().size()); // should prob. be no. of nodes in corresponding model subgraph

	// synch model graph with changes
	config.Update(Q);
	loops.Field(dgr::dynadag,"model nodes",model.nodes().size());
	timer.LoopPoint(dgr::timing,"update model graph");

	if(gd<GraphGeom>(&world_->current_).reportIntermediate) {
		generateIntermediateLayout(Q);
		NextProcess();
		// client has heard about inserts so they're now mods, 
		// and deletes can be blown away (should someone Higher do this?)
		ClearInsDel(Q);
		// and don't overreport attr changes
		ClearStrAttrChanges(Q);
	}

	if(gd<Interruptible>(&world_->current_).interrupt 
			&& gd<GraphGeom>(&world_->current_).reportIntermediate
			&& g_dynadagPhaseMinder.Check(gd<Interruptible>(&world_->current_).attrs,"update","done")) {
		rememberOld();
		StrAttrs pulseAttrs;
		pulseAttrs["phase"] = "update";
		pulseAttrs["step"] = "done";
		NextPulse(pulseAttrs);
		return;
	}

	// crossing optimization
	optimizer->Reorder(world_->current_,world_->current_);//crossN,crossE);
	timer.LoopPoint(dgr::timing,"crossing optimization");

	if(gd<Interruptible>(&world_->current_).interrupt 
			&& gd<GraphGeom>(&world_->current_).reportIntermediate
			&& g_dynadagPhaseMinder.Check(gd<Interruptible>(&world_->current_).attrs,"untangle","done")) {
		//makeXConsistent(); // this horror superceded by horrible x_backup 
		rememberOld();
		StrAttrs pulseAttrs;
		pulseAttrs["phase"] = "untangle";
		pulseAttrs["step"] = "done";
		NextPulse(pulseAttrs);
		return;
	}

	// find X coords
	xsolver.Place(Q);

	// (with ConseqRanks, find rank Ys from node heights.)  find node Y coords from rank Ys.
	config.SetYs();
	timer.LoopPoint(dgr::timing,"optimize x coordinates");

	if(gd<Interruptible>(&world_->current_).interrupt 
			&& gd<GraphGeom>(&world_->current_).reportIntermediate
			&& g_dynadagPhaseMinder.Check(gd<Interruptible>(&world_->current_).attrs,"xopt","done")) {
		rememberOld();
		StrAttrs pulseAttrs;
		pulseAttrs["phase"] = "xopt";
		pulseAttrs["step"] = "done";
		NextPulse(pulseAttrs);
		return;
	}

	// calculate bounding rectangle
	updateBounds(Q);

	// find node & edge moves
	findChangedNodes(Q);

	findDirtyEdges(Q,ModifyFlags(Q).flags&DG_UPD_EDGESTYLE);
	findFlowSlopes(Q);
	redrawEdges(Q);

	timer.LoopPoint(dgr::timing,"draw splines");

	// reset flags
	rememberOld();

	dumpModel();

	loops.Field(dgr::dynadag,"nodes inserted - output",Q.insN.nodes().size());
	loops.Field(dgr::dynadag,"edges inserted - output",Q.insE.edges().size());
	loops.Field(dgr::dynadag,"nodes modified - output",Q.modN.nodes().size());
	loops.Field(dgr::dynadag,"edges modified - output",Q.modE.edges().size());
	loops.Field(dgr::dynadag,"nodes deleted - output",Q.delN.nodes().size());
	loops.Field(dgr::dynadag,"edges deleted - output",Q.delE.nodes().size());
	if(reports.enabled(dgr::readability)) {
		Crossings cc = calculateCrossings(config);
		loops.Field(dgr::readability,"edge-edge crossings",cc.edgeEdgeCross);
		loops.Field(dgr::readability,"edge-node crossings",cc.nodeEdgeCross);
		loops.Field(dgr::readability,"node-node crossings",cc.nodeNodeCross);

		pair<int,Coord> elen = calculateTotalEdgeLength(config);
		Coord avg = elen.second/elen.first;
		loops.Field(dgr::readability,"average edge x-length",avg.x);
		loops.Field(dgr::readability,"average edge y-length",avg.y);
	}
	NextProcess();
	{
		// this is weird but helpful..?
		StrAttrs pulseAttrs;
		pulseAttrs["phase"] = "finish";
		pulseAttrs["step"] = "done";
		NextPulse(pulseAttrs);
	}
	// we are good with all changes now
	Q.ExecuteDeletions();
	Q.Clear();
}

} // namespace DynaDAG
} // namespace Dynagraph
