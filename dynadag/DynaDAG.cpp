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
#include "common/PhaseMinder.h"
#include "common/ChangeTools.h"

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
	double glb=0.0,grb=0.0;  
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
	if(assign_unclose(gd<GraphGeom>(&world_->current_).bounds,bb))
		ModifyFlags(changeQ) |= DG_UPD_BOUNDS;
}
void DynaDAGServer::moveNodesBasedOnModel(DDChangeQueue &changeQ) {
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
void DynaDAGServer::generateIntermediateLayout(DDChangeQueue &changeQ) {
	moveNodesBasedOnModel(changeQ);
	for(DynaDAGLayout::graphedge_iter ei = changeQ.insE.edges().begin(); ei!=changeQ.insE.edges().end(); ++ei)
		if(!gd<NSRankerEdge>(*ei).secondOfTwo)
			drawEdgeSimply(*ei);
	for(DynaDAGLayout::graphedge_iter ei = changeQ.modE.edges().begin(); ei!=changeQ.modE.edges().end(); ++ei)
		if(!gd<NSRankerEdge>(*ei).secondOfTwo && igd<Update>(*ei).flags & DG_UPD_MOVE)
			drawEdgeSimply(*ei);
	drawSecondEdges(changeQ);
}
void DynaDAGServer::rememberOld() { 
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
			&& g_dynadagPhaseMinder.HasPassed(gd<Interruptible>(&world_->current_).attrs,"update","done")) {
		rememberOld();
		StrAttrs pulseAttrs;
		pulseAttrs["phase"] = "update";
		pulseAttrs["step"] = "done";
		NextPulse(pulseAttrs);
		return;
	}

	// crossing optimization
	optimizer->Reorder(Q,world_->current_,world_->current_);
	timer.LoopPoint(dgr::timing,"crossing optimization");

	if(gd<Interruptible>(&world_->current_).interrupt 
			&& gd<GraphGeom>(&world_->current_).reportIntermediate
			&& g_dynadagPhaseMinder.HasPassed(gd<Interruptible>(&world_->current_).attrs,"untangle","done")) {
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
			&& g_dynadagPhaseMinder.HasPassed(gd<Interruptible>(&world_->current_).attrs,"xopt","done")) {
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
	moveNodesBasedOnModel(Q);
	findFlowSlopes(Q);
	redrawEdges(Q,ModifyFlags(Q).flags&DG_UPD_EDGESTYLE);

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
