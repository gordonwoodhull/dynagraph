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

namespace Dynagraph {
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
	DynaDAGLayout::Edge *layoutE = path->layoutE;
	gd<EdgeGeom>(layoutE).constraint = true;

	DDCGraph::Node *tvar = cg.GetVar(DDp(layoutE->tail)->bottomC),
		*hvar = cg.GetVar(DDp(layoutE->head)->topC);

	DDCGraph::Edge *constr = cg.create_edge(tvar,hvar).first;
	path->strong = constr;
	DDNS::NSE &nse = DDNS::NSd(constr);
	double length = max(0.,gd<EdgeGeom>(layoutE).minLength);
#ifdef FLEXIRANKS
	nse.minlen = ROUND(gd<GraphGeom>(config.whole).separation.y*length/config.ranking.div);
#else
	nse.minlen = length;
#endif
	nse.weight = EDGELENGTH_WEIGHT;
}
void Ranker::makeWeakConstraint(DDPath *path) {
	assert(!path->weak);
	DynaDAGLayout::Edge *layoutE = path->layoutE;
	gd<EdgeGeom>(layoutE).constraint = false;

	DDCGraph::Node *tvar = cg.GetVar(DDp(layoutE->tail)->bottomC),
		*hvar = cg.GetVar(DDp(layoutE->head)->topC);
	path->weak = cg.create_node();
	gd<ConstraintType>(path->weak).why = ConstraintType::rankWeak;
	NSEdgePair ep(path->weak,tvar,hvar);
	DDNS::NSd(ep.e[0]).minlen = 0;
	DDNS::NSd(ep.e[0]).weight = BACKEDGE_PENALTY;
	double length = max(0.,gd<EdgeGeom>(layoutE).minLength);
#ifdef FLEXIRANKS
	DDNS::NSd(ep.e[1]).minlen = ROUND(gd<GraphGeom>(config.whole).separation.y*length/config.ranking.div);
#else
	DDNS::NSd(ep.e[1]).minlen = length;
#endif
}
// change edge strengths around a node
void Ranker::fixNode(DynaDAGLayout::Node *n,bool fix) {
	for(DynaDAGLayout::nodeedge_iter ei = n->alledges().begin(); ei!=n->alledges().end(); ++ei) {
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
void Ranker::doNodeHeight(DynaDAGLayout::Node *n) {
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
void Ranker::moveOldNodes(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::node_iter ni = changeQ.modN.nodes().begin(); ni!=changeQ.modN.nodes().end(); ++ni) {
		DDMultiNode *n = DDp(*ni);
		Update upd = igd<Update>(*ni);
		if(upd.flags & DG_UPD_NAIL) {
			DynaDAGLayout::Node *vn = config.current->find(*ni);
			if(gd<NodeGeom>(*ni).nail & DG_NAIL_Y) {
				if(!gd<NodeGeom>(*ni).pos.valid)
					throw NailWithoutPos(*ni);
				if(!n->rankFixed)
					fixNode(vn,true);
			}
			else if(n->rankFixed) // un-nail
				fixNode(vn,false);
		}
		if(upd.flags & DG_UPD_REGION)
			doNodeHeight(*ni);
	}
}
void Ranker::insertNewNodes(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::node_iter ni = changeQ.insN.nodes().begin(); ni!=changeQ.insN.nodes().end(); ++ni) {
        DynaDAGLayout::Node *n = *ni;
		DDModel::Node *mn = dynaDAG->OpenModelNode(config.whole->find(n)).second;

		// pull loose nodes upward
		ConstraintGraph::Edge *pull = cg.create_edge(top,cg.GetVar(DDd(mn).multi->topC)).first;
		DDNS::NSd(pull).minlen = 0;
		DDNS::NSd(pull).weight = UPWARD_TENDENCY;

		doNodeHeight(n);
	}
}
void Ranker::stabilizePositionedNodes(DDChangeQueue &changeQ) {
	// all nodes with position should want to stay there
	// (this is particularly important for nodes that started without a position but got one last round)
	for(DynaDAGLayout::node_iter ni = config.current->nodes().begin(); ni!=config.current->nodes().end(); ++ni)
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
static int dfs(DynaDAGLayout::Node *src, DynaDAGLayout::Node *dest, LNodeV &hit) {
    if(src == dest) {
		return true;
	}
    if(DDp(src)->hit)
		return false;
	DDp(src)->hit = true;
	hit.push_back(src);
    for(DynaDAGLayout::outedge_iter ei = src->outs().begin(); ei!=src->outs().end(); ++ei) {
		if(!DDp(*ei) || !DDp(*ei)->strong)
			continue;
        if(dfs((*ei)->head,dest,hit))
			return true;
    }
    return false;
}
static int pathExists(DynaDAGLayout::Node *src, DynaDAGLayout::Node *dest) {
    LNodeV hit;
    bool result = dfs(src,dest,hit);
	for(LNodeV::iterator i=hit.begin(); i!=hit.end(); ++i)
		DDp(*i)->hit = false;
	return result;
}
void Ranker::insertNewEdges(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::graphedge_iter ei = changeQ.insE.edges().begin(); ei!=changeQ.insE.edges().end(); ++ei) {
		DynaDAGLayout::Edge *ve = *ei;
		// set up path but don't model it yet (that's config's job)
		DynaDAGLayout *current = config.current;
		DDPath *path = dynaDAG->OpenModelEdge(0,0,config.whole->find(ve)).first;
		if(ve->head == ve->tail)
			continue;
		bool weak = false;
		if(DynaDAGLayout::Edge *e1 = current->find_edge(ve->head,ve->tail)) {
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
/* special-case when it is easy (INACTIVE; WOULD NEED FIXES) */
bool Ranker::simpleCase(DDChangeQueue &changeQ) {
	/* there must be no deletions */
	if(changeQ.delN.nodes().size() || changeQ.delE.nodes().size())
		return false;

	/* there must be no mods */
	if(changeQ.modN.nodes().size() || changeQ.modE.nodes().size())
		return false;

	/* there can only be singleton or leaf node insertions */
	for(DynaDAGLayout::graphedge_iter ei = changeQ.insE.edges().begin(); ei!=changeQ.insE.edges().end(); ++ei) {
		DynaDAGLayout::Edge *e = *ei;
		DynaDAGLayout::Node *u = e->tail,
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
	for(DynaDAGLayout::node_iter ni = changeQ.insN.nodes().begin(); ni!=changeQ.insN.nodes().end(); ++ni) {
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

		if(DynaDAGLayout::Node *u = changeQ.insE.find(*ni)) {
			if(u->ins().size()) {
				DynaDAGLayout::Edge *e = *u->ins().begin();
				if(!ng.pos.valid || gd<EdgeGeom>(e).constraint) {
					// this use of minLength ...
					int r = DDd(DDp(e->tail)->bottom()).rank + (int)gd<EdgeGeom>(e).minLength;
					if(!ng.pos.valid || r > newTopRank)
						newTopRank = r;
				}
			}
			else if(u->outs().size()) {
				DynaDAGLayout::Edge *e = *u->outs().begin();
				if(!ng.pos.valid || gd<EdgeGeom>(e).constraint) {
					// ... and this one would would have to fixed if this function was enabled
					int r = DDd(DDp(e->head)->top()).rank - (int)gd<EdgeGeom>(e).minLength;
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
void Ranker::recomputeRanks(DDChangeQueue &changeQ) {
	/*
	if(simpleCase(changeQ))
		return;
		*/
 	DDNS().Solve(&cg,NS::NORMALIZE|NS::RECHECK|NS::VALIDATE|NS::ATTACHATTRS);
	int anchorRank = DDNS::NSd(cg.anchor).rank;
	for(DynaDAGLayout::node_iter ni = config.current->nodes().begin(); ni!=config.current->nodes().end(); ++ni) {
		DynaDAGLayout::Node *n = *ni;
		if(changeQ.delN.find(n))
			continue;
		int newTopRank = DDNS::NSd(DDp(n)->topC.n).rank - anchorRank,
			newBottomRank = DDNS::NSd(DDp(n)->bottomC.n).rank - anchorRank;
		assert(!config.ranking.Below(newTopRank,newBottomRank));
		DDMultiNode *mn = DDp(n);
		if(newTopRank != mn->oldTopRank || newBottomRank != mn->oldBottomRank) {
			mn->newTopRank = newTopRank;
			mn->newBottomRank = newBottomRank;
			ModifyNode(changeQ,n,DG_UPD_MOVE);
			DynaDAGLayout::Node *cn = config.current->find(n);
			for(DynaDAGLayout::nodeedge_iter ei=cn->alledges().begin(); ei!=cn->alledges().end(); ++ei)
				ModifyEdge(changeQ,*ei,DG_UPD_MOVE);
		}
	}
}
#ifdef FLEXIRANKS
void Ranker::makeRankList(DDChangeQueue &changeQ) {
	Config::Ranks::IndexV &newRanks = config.ranking.newRanks;
	// the ranks consist of tops of nodes, bottoms of nodes, 
	// and places where phantom nodes for S-shaped back-edges, 
	// fanning order, and parallel/2-cycle edges go
	for(DynaDAGLayout::node_iter ni = config.current->nodes().begin(); ni!=config.current->nodes().end(); ++ni)
		if(!changeQ.delN.find(*ni)) {
			newRanks.push_back(DDp(*ni)->newTopRank);
			newRanks.push_back(DDp(*ni)->newBottomRank);
		}
	sort(newRanks.begin(),newRanks.end());
	Config::Ranks::IndexV::iterator uniquend = unique(newRanks.begin(),newRanks.end());
	newRanks.resize(uniquend-newRanks.begin());
}
#endif
void Ranker::findEdgeDirections() {
	for(DynaDAGLayout::graphedge_iter ei = config.current->edges().begin(); ei!=config.current->edges().end(); ++ei) {
		int tlr = DDp((*ei)->tail)->newBottomRank,
			hdr = DDp((*ei)->head)->newTopRank;
		if(tlr==hdr)
			DDp(*ei)->direction = DDPath::flat;
		else if(tlr>hdr) {
			tlr = DDp((*ei)->head)->newBottomRank;
			hdr = DDp((*ei)->tail)->newTopRank;
			if(tlr>hdr)
				DDp(*ei)->direction = DDPath::flat;
			else
				DDp(*ei)->direction = DDPath::reversed;
		}
		else
			DDp(*ei)->direction = DDPath::forward;
	}

}
void Ranker::Rerank(DDChangeQueue &changeQ) {
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
	findEdgeDirections();
	//checkStrongConstraints(changeQ);
}

} // namespace DynaDAG
} // namespace Dynagraph
