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

namespace DynaDAG {

void Ranker::RemoveLayoutNodeConstraints(DDMultiNode *m) {
	cg.RemoveNodeConstraints(m->topC);
	cg.RemoveNodeConstraints(m->bottomC);
}
void Ranker::RemovePathConstraints(DDPath *path) {
	if(path->weak) {
		cg.erase_node(path->weak);
		path->weak = 0;
	}
	if(path->strong) {
		cg.erase_edge(path->strong);
		path->strong = 0;
	}
	gd<EdgeGeom>(path->layoutE).constraint = false;
}
void Ranker::makeStrongConstraint(DDPath *path) {
	assert(!path->strong);
	Layout::Edge *layoutE = path->layoutE;
	gd<EdgeGeom>(layoutE).constraint = true;

	int length = rankLength(layoutE);
	DDCGraph::Node *tvar = cg.GetVar(DDp(layoutE->tail)->bottomC),
		*hvar = cg.GetVar(DDp(layoutE->head)->topC);

	DDCGraph::Edge *constr = cg.create_edge(tvar,hvar).first;
	path->strong = constr;
	DDNS::NSE &nse = DDNS::NSd(constr);
#ifdef FLEXIRANKS
	nse.minlen = ROUND(gd<GraphGeom>(config.client).separation.y*length/config.ranking.div);
#else
	nse.minlen = rankLength(layoutE);
#endif
	nse.weight = EDGELENGTH_WEIGHT;
}
void Ranker::makeWeakConstraint(DDPath *path) {
	assert(!path->weak);
	Layout::Edge *layoutE = path->layoutE;
	gd<EdgeGeom>(layoutE).constraint = false;

	DDCGraph::Node *tvar = cg.GetVar(DDp(layoutE->tail)->bottomC),
		*hvar = cg.GetVar(DDp(layoutE->head)->topC);
	path->weak = cg.create_node();
	gd<ConstraintType>(path->weak).why = ConstraintType::rankWeak;
	NSEdgePair ep(path->weak,tvar,hvar);
	DDNS::NSd(ep.e[0]).minlen = 0;
	DDNS::NSd(ep.e[0]).weight = BACKEDGE_PENALTY;
#ifdef FLEXIRANKS
	DDNS::NSd(ep.e[1]).minlen = ROUND(gd<GraphGeom>(config.client).separation.y*rankLength(layoutE)/config.ranking.div);
#else
	DDNS::NSd(ep.e[1]).minlen = rankLength(layoutE);
#endif
}
// change edge strengths around a node
void Ranker::fixNode(Layout::Node *n,bool fix) {
	for(Layout::nodeedge_iter ei = n->alledges().begin(); ei!=n->alledges().end(); ++ei) {
		DDPath *path = DDp(*ei);
		if(path->strong && fix) {
			RemovePathConstraints(path);
			makeWeakConstraint(path);
		}
		else if(path->weak && !fix) {
			RemovePathConstraints(path);
			makeStrongConstraint(path);
		}
	}
	DDp(n)->rankFixed = fix;
}
void Ranker::doNodeHeight(Layout::Node *n) {
	DDMultiNode *mn = DDp(n);
	DDCGraph::Node *tv = cg.GetVar(mn->topC),
		*bv = cg.GetVar(mn->bottomC);
	DDCGraph::Edge *heightC = cg.create_edge(tv,bv).first;
#ifdef FLEXIRANKS
	 // one-node chains cause trouble; make sure there's one edge
	DDNS::NSd(heightC).minlen = max(1,ROUND(gd<NodeGeom>(n).region.boundary.Height()/config.ranking.div));
#else
	DDNS::NSd(heightC).minlen = 0;
#endif
	DDNS::NSd(heightC).weight = NODEHEIGHT_PENALTY;
}
void Ranker::moveOldNodes(ChangeQueue &changeQ) {
	for(Layout::node_iter ni = changeQ.modN.nodes().begin(); ni!=changeQ.modN.nodes().end(); ++ni) {
		DDMultiNode *n = DDp(*ni);
		unsigned upd = igd<Update>(*ni).flags;
		if(upd & DG_UPD_NAIL) {
			Layout::Node *vn = config.current->find(*ni);
			if(gd<NodeGeom>(*ni).nail & DG_NAIL_Y) {
				if(!gd<NodeGeom>(*ni).pos.valid)
					throw NailWithoutPos(*ni);
				if(!n->rankFixed)
					fixNode(vn,true);
			}
			else if(n->rankFixed) // un-nail
				fixNode(vn,false);
		}
		if(upd & DG_UPD_REGION)
			doNodeHeight(*ni);
	}
}
void Ranker::insertNewNodes(ChangeQueue &changeQ) {
	for(Layout::node_iter ni = changeQ.insN.nodes().begin(); ni!=changeQ.insN.nodes().end(); ++ni) {
        Layout::Node *n = *ni;
		DDModel::Node *mn = dynaDAG->OpenModelNode(config.client->find(n)).second;

		// pull loose nodes upward
		ConstraintGraph::Edge *pull = cg.create_edge(top,cg.GetVar(DDd(mn).multi->topC)).first;
		DDNS::NSd(pull).minlen = 0;
		DDNS::NSd(pull).weight = UPWARD_TENDENCY;

		doNodeHeight(n);
	}
}
void Ranker::stabilizePositionedNodes(ChangeQueue &changeQ) {
	// all nodes with position should want to stay there
	// (this is particularly important for nodes that started without a position but got one last round)
	for(Layout::node_iter ni = config.current->nodes().begin(); ni!=config.current->nodes().end(); ++ni)
		if(!changeQ.delN.find(*ni)) {
			DDMultiNode *mn = DDp(*ni);
			// stabilize at starting position if any
			if(gd<NodeGeom>(*ni).pos.valid) {
				int topRank = config.ranking.MapCoordToRank(gd<NodeGeom>(*ni).BoundingBox().t);
				cg.Stabilize(mn->topC,topRank,STABILITY_FACTOR_Y);
			}
			else
				cg.Unstabilize(mn->topC);
		}
}
/* is there a path from head(e) to tail(e)? next two fns. */
static int dfs(Layout::Node *src, Layout::Node *dest, LNodeV &hit) {
    if(src == dest) {
		return true;
	}
    if(DDp(src)->hit)
		return false;
	DDp(src)->hit = true;
	hit.push_back(src);
    for(Layout::outedge_iter ei = src->outs().begin(); ei!=src->outs().end(); ++ei) {
		if(!DDp(*ei) || !DDp(*ei)->strong)
			continue;
        if(dfs((*ei)->head,dest,hit))
			return true;
    }
    return false;
}
static int pathExists(Layout::Node *src, Layout::Node *dest) {
    LNodeV hit;
    bool result = dfs(src,dest,hit);
	for(LNodeV::iterator i=hit.begin(); i!=hit.end(); ++i)
		DDp(*i)->hit = false;
	return result;
}
void Ranker::insertNewEdges(ChangeQueue &changeQ) {
	for(Layout::graphedge_iter ei = changeQ.insE.edges().begin(); ei!=changeQ.insE.edges().end(); ++ei) {
		Layout::Edge *ve = *ei;
		// set up path but don't model it yet (that's config's job)
		Layout *current = config.current;
		DDPath *path = dynaDAG->OpenModelEdge(0,0,config.client->find(ve)).first;
		if(ve->head == ve->tail)
			continue;
		bool weak = false;
		if(Layout::Edge *e1 = current->find_edge(ve->head,ve->tail)) {
			// mark & ignore second leg of 2-cycle for all modelling purposes
			// DynaDAGServer will draw it by reversing the other
			if(DDp(e1) && (DDp(e1)->weak || DDp(e1)->strong)) { // if both get inserted at once, mark the second
				DDp(ve)->secondOfTwo = true;
				continue;
			}
		}
		else if(pathExists(current->find(ve->head),current->find(ve->tail)))
			weak = true;
		if(DDp(ve->tail)->rankFixed || DDp(ve->head)->rankFixed)
			weak = true;
		if(weak)
			makeWeakConstraint(path);
		else
			makeStrongConstraint(path);
		if(config.current->find(ve->head)->degree()==1)
			cg.Unstabilize(DDp(ve->head)->topC);
		if(config.current->find(ve->tail)->degree()==1)
			cg.Unstabilize(DDp(ve->tail)->topC);
	}
}
/* special-case when it is easy */
bool Ranker::simpleCase(ChangeQueue &changeQ) {
	/* there must be no deletions */
	if(changeQ.delN.nodes().size() || changeQ.delE.nodes().size())
		return false;

	/* there must be no mods */
	if(changeQ.modN.nodes().size() || changeQ.modE.nodes().size())
		return false;

	/* there can only be singleton or leaf node insertions */
	for(Layout::graphedge_iter ei = changeQ.insE.edges().begin(); ei!=changeQ.insE.edges().end(); ++ei) {
		Layout::Edge *e = *ei;
		Layout::Node *u = e->tail,
			*v = e->head,
			*possibleLeaf,
			*other;
		if(changeQ.insN.find(u)) {
			possibleLeaf = u;
			other = v;
		}
		else if(changeQ.insN.find(v)) {
			possibleLeaf = v;
			other = u;
		}
		else return false;	/* edge connects old nodes */
		/* test if edge connects two new nodes */
		if(changeQ.insN.find(other))
			return false;
		/* test if new node is not a leaf */
		if(possibleLeaf->degree() > 1)
			return false;
	}

	/* do the insertions */
	for(Layout::node_iter ni = changeQ.insN.nodes().begin(); ni!=changeQ.insN.nodes().end(); ++ni) {
		NodeGeom &ng = gd<NodeGeom>(*ni);
		int newTopRank;
		Bounds b = ng.BoundingBox();
		if(b.valid)
#ifdef FLEXIRANKS
			newTopRank = config.ranking.MapCoordToRank(b.t);
#else
			newTopRank = config.ranking.MapCoordToRank(ng.pos.y);
#endif
		else
			newTopRank = config.ranking.Low();

		if(Layout::Node *u = changeQ.insE.find(*ni)) {
			if(u->ins().size()) {
				Layout::Edge *e = *u->ins().begin();
				if(!ng.pos.valid || gd<EdgeGeom>(e).constraint) {
					int r = DDd(DDp(e->tail)->bottom()).rank + rankLength(e);
					if(!ng.pos.valid || r > newTopRank)
						newTopRank = r;
				}
			}
			else if(u->outs().size()) {
				Layout::Edge *e = *u->outs().begin();
				if(!ng.pos.valid || gd<EdgeGeom>(e).constraint) {
					int r = DDd(DDp(e->head)->top()).rank - rankLength(e);
					if(!ng.pos.valid || r < newTopRank)
						newTopRank = r;
				}
			}
			else abort();	/* where is the edge? */
		}
		else { /* singleton */ }
		DDp(*ni)->newTopRank = newTopRank;
#ifdef FLEXIRANKS
		if(b.valid) {
			config.ranking.EnsureRank(newTopRank);
			DDp(*ni)->newBottomRank = config.ranking.MapCoordToRank(config.ranking.GetRank(newTopRank)->yBase
#ifndef DOWN_GREATER
				-
#else
				+
#endif
				b.Height());
			assert(!config.ranking.Below(DDp(*ni)->newTopRank,DDp(*ni)->newBottomRank));
		}
		else
#endif
			DDp(*ni)->newBottomRank = newTopRank;
	}
	return true;
}
void Ranker::recomputeRanks(ChangeQueue &changeQ) {
	/*
	if(simpleCase(changeQ))
		return;
		*/
 	DDNS().Solve(&cg,NS::NORMALIZE|NS::RECHECK|NS::VALIDATE|NS::ATTACHATTRS);
	int anchorRank = DDNS::NSd(cg.anchor).rank;
	for(Layout::node_iter ni = config.current->nodes().begin(); ni!=config.current->nodes().end(); ++ni) {
		Layout::Node *n = *ni;
		if(changeQ.delN.find(n))
			continue;
		int newTopRank = DDNS::NSd(DDp(n)->topC.n).rank - anchorRank,
			newBottomRank = DDNS::NSd(DDp(n)->bottomC.n).rank - anchorRank;
		assert(!config.ranking.Below(newTopRank,newBottomRank));
		DDMultiNode *mn = DDp(n);
		if(newTopRank != mn->oldTopRank || newBottomRank != mn->oldBottomRank) {
			mn->newTopRank = newTopRank;
			mn->newBottomRank = newBottomRank;
			changeQ.ModNode(n,DG_UPD_MOVE);
			Layout::Node *cn = config.current->find(n);
			for(Layout::nodeedge_iter ei=cn->alledges().begin(); ei!=cn->alledges().end(); ++ei)
				changeQ.ModEdge(*ei,DG_UPD_MOVE);
		}
	}
}
#ifdef FLEXIRANKS
void Ranker::makeRankList(ChangeQueue &changeQ) {
	Config::Ranks::IndexV rankvec;
	for(Layout::node_iter ni = config.current->nodes().begin(); ni!=config.current->nodes().end(); ++ni)
		if(!changeQ.delN.find(*ni)) {
			rankvec.push_back(DDp(*ni)->newTopRank);
			rankvec.push_back(DDp(*ni)->newBottomRank);
		}
	sort(rankvec.begin(),rankvec.end());
	config.ranking.newRanks.clear();
	for(Config::Ranks::IndexV::iterator ii = rankvec.begin(); ii!=rankvec.end(); ++ii)
		if(config.ranking.newRanks.empty()||*ii!=config.ranking.newRanks.back())
			config.ranking.newRanks.push_back(*ii);
}
#endif
void Ranker::Rerank(ChangeQueue &changeQ) {
	// this connection is just to keep the graph connected
	ConstraintGraph::Edge *c = cg.create_edge(top,cg.anchor).first;
	DDNS::NSd(c).minlen = DDNS::NSd(c).weight = 0;

	moveOldNodes(changeQ);
	insertNewNodes(changeQ);
	insertNewEdges(changeQ);
	stabilizePositionedNodes(changeQ);
	recomputeRanks(changeQ);
#ifdef FLEXIRANKS
	makeRankList(changeQ);
#endif
	//checkStrongConstraints(changeQ);
}

} // namespace DynaDAG
