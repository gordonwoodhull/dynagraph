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

#ifndef NSRanker_h
#define NSRanker_h

#include "NSRankerAttrs.h"

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout>
struct NSRanker : LinkedChangeProcessor<Layout> {
	NSRanker(ChangingGraph<Graph> *world) 
		: LinkedChangeProcessor<Graph>(world),
		rankXlate_(gd<GraphGeom>(world->current).resolution.y),
		current_(world->current)
	{}
	~NSRanker();
	void Process();
private:
	FlexiRankXlator rankXlate_;
	Layout *current_;
	void removeLayoutNodeConstraints(typename Layout::Node *m);
	void removePathConstraints(typename Layout::Edge *e);
	void removeOldConstraints(ChangeQueue<Layout> &changeQ,Layout &extraI);
	void makeStrongConstraint(typename Layout::Edge *e);
	void makeWeakConstraint(typename Layout::Edge *e);
	void fixNode(typename Layout::Node *n,bool fix);
	void doNodeHeight(typename Layout::Node *n);
	void moveOldNodes(ChangeQueue<Layout> &changeQ);
	void insertNewNodes(ChangeQueue<Layout> &changeQ);
	void stabilizePositionedNodes(ChangeQueue<Layout> &changeQ);
	void insertNewEdges(Layout &insE);
	void recomputeRanks(ChangeQueue<Layout> &changeQ);
};




// run a mass delete?

template<typename Layout>
NSRanker<Layout>::~NSRanker() {
	for(typename Layout::graphedge_iter i = current_->edges().begin();i!=current_->edges().end();++i)
		removePathConstraints(*i);
	for(typename Layout::node_iter j = current_->nodes().begin(); j!=current_->nodes().end(); ++j)
		removeLayoutNodeConstraints(*j);
}


// change edge strengths around a node
template<typename Layout>
void NSRanker<Layout>::fixNode(typename Layout::Node *n,bool fix) {
	for(typename Layout::nodeedge_iter ei = n->alledges().begin(); ei!=n->alledges().end(); ++ei) {
		typename Layout::Edge *e = *ei;
		if(gd<NSRankerEdge>(e).strong && fix) {
			removePathConstraints(e);
			makeWeakConstraint(e);
		}
		else if(gd<NSRankerEdge>(e).weak && !fix) {
			removePathConstraints(e);
			makeStrongConstraint(e);
		}
	}
	gd<NSRankerNode>(n).rankFixed = fix;
}



// to LayoutToNSRankerModelTranslator

template<typename Layout>
void NSRanker<Layout>::moveOldNodes(ChangeQueue<Layout> &changeQ) {
	for(typename Layout::node_iter ni = changeQ.modN.nodes().begin(); ni!=changeQ.modN.nodes().end(); ++ni) {
		typename Layout::Node *n = *ni;
		Update upd = igd<Update>(n);
		if(upd.flags & DG_UPD_NAIL) {
			if(gd<NodeGeom>(n).nail & DG_NAIL_Y) {
				if(!gd<NodeGeom>(n).pos.valid)
					throw NailWithoutPos<Layout>(n);
				if(!gd<NSRankerNode>(n).rankFixed)
					fixNode(n,true);
			}
			else if(gd<NSRankerNode>(n).rankFixed) // un-nail
				fixNode(n,false);
		}
		if(upd.flags & DG_UPD_REGION)
			doNodeHeight(n);
	}
}



// to NSRankerModelToConstraintTranslator



// (may need to figure this out in LayoutToNSRankerModel and set a stabilize attr)
template<typename Layout>
void NSRanker<Layout>::stabilizePositionedNodes(ChangeQueue<Layout> &changeQ) {
	// all nodes with position should want to stay there
	// (this is particularly important for nodes that started without a position but got one last round)
	for(typename Layout::node_iter ni = changeQ.current->nodes().begin(); ni!=changeQ.current->nodes().end(); ++ni) {
        typename Layout::Node *n = *ni;
		if(!changeQ.delN.find(n)) {
			// stabilize at starting position if any
			if(gd<NodeGeom>(n).pos.valid) {
				int topRank = rankXlate_.CoordToRank(gd<NodeGeom>(n).BoundingBox().t);
				cg_.Stabilize(gd<NSRankerNode>(n).topC,topRank,STABILITY_FACTOR_Y);
			}
			else
				cg_.Unstabilize(gd<NSRankerNode>(n).topC);
		}
	}
}



// this broken code replace with NSRankerSimpleCycleBreaker

/* is there a path from head(e) to tail(e)? next two fns. */
template<typename Layout>
int dfs(typename Layout::Node *src, typename Layout::Node *dest, std::vector<typename Layout::Node*> &hit) {
    if(src == dest) {
		return true;
	}
    if(gd<NSRankerNode>(src).hit)
		return false;
	gd<NSRankerNode>(src).hit = true;
	hit.push_back(src);
    for(typename Layout::outedge_iter ei = src->outs().begin(); ei!=src->outs().end(); ++ei) {
		if(!gd<NSRankerEdge>(*ei).strong)
			continue;
        if(dfs<Layout>((*ei)->head,dest,hit))
			return true;
    }
    return false;
}
template<typename Layout>
int pathExists(typename Layout::Node *src, typename Layout::Node *dest) {
    std::vector<typename Layout::Node*> hit;
    bool result = dfs<Layout>(src,dest,hit);
	for(typename std::vector<typename Layout::Node*>::iterator i=hit.begin(); i!=hit.end(); ++i)
		gd<NSRankerNode>(*i).hit = false;
	return result;
}

// this gets split between LayoutToNSRankerModel and NSRankerSimpleCycleBreaker (and NSRankerModelToConstraint does the actual constraint building)
template<typename Layout>
void NSRanker<Layout>::insertNewEdges(Layout &insE) {
	for(typename Layout::graphedge_iter ei = insE.edges().begin(); ei!=insE.edges().end(); ++ei) {
		typename Layout::Edge *e = *ei;
		if(e->head == e->tail)
			continue;
		bool weak = false;
		if(typename Layout::Edge *e1 = current_->find_edge(e->head,e->tail)) {
			// mark & ignore second leg of 2-cycle for all modeling purposes
			// DynaDAGServer will draw it by reversing the other
			// if both get inserted at once, mark the second processed here (should be 2nd inserted)
			if(gd<NSRankerEdge>(e1).weak || gd<NSRankerEdge>(e1).strong) {
				gd<NSRankerEdge>(e).secondOfTwo = true;
				continue;
			}
		}
		else if(pathExists<Layout>(current_->find(e->head),current_->find(e->tail)))
			weak = true;
		if(gd<NSRankerNode>(e->tail).rankFixed || gd<NSRankerNode>(e->head).rankFixed)
			weak = true;
		if(weak)
			makeWeakConstraint(e);
		else
			makeStrongConstraint(e);
		if(current_->find(e->head)->degree()==1)
			cg_.Unstabilize(gd<NSRankerNode>(e->head).topC);
		if(current_->find(e->tail)->degree()==1)
			cg_.Unstabilize(gd<NSRankerNode>(e->tail).topC);
	}
}




// this has already been moved to NSRankerModelToLayout

template<typename Layout>
void NSRanker<Layout>::recomputeRanks(ChangeQueue<Layout> &changeQ) {
 	DDNS().Solve(&cg_,NS::NORMALIZE|NS::RECHECK|NS::VALIDATE|NS::ATTACHATTRS);
	int anchorRank = DDNS::NSd(cg_.anchor).rank;
	for(typename Layout::node_iter ni = changeQ.current->nodes().begin(); ni!=changeQ.current->nodes().end(); ++ni) {
		typename Layout::Node *n = *ni;
		if(changeQ.delN.find(n))
			continue;
		int newTopRank = DDNS::NSd(gd<NSRankerNode>(n).topC.n).rank - anchorRank,
			newBottomRank = DDNS::NSd(gd<NSRankerNode>(n).bottomC.n).rank - anchorRank;
		assert(!rankXlate_.Below(newTopRank,newBottomRank));
		if(newTopRank != gd<NSRankerNode>(n).oldTopRank || newBottomRank != gd<NSRankerNode>(n).oldBottomRank) {
			gd<NSRankerNode>(n).newTopRank = newTopRank;
			gd<NSRankerNode>(n).newBottomRank = newBottomRank;
			ModifyNode(changeQ,n,DG_UPD_MOVE);
			typename Layout::Node *cn = changeQ.current->find(n);
			for(typename Layout::nodeedge_iter ei=cn->alledges().begin(); ei!=cn->alledges().end(); ++ei)
				ModifyEdge(changeQ,*ei,DG_UPD_MOVE);
		}
	}
}
template<typename Layout>
void NSRanker<Layout>::Process() {
	ChangeQueue<Layout> &Q = this->world_->Q_;
	// this connection is just to keep the graph connected
	ConstraintGraph::Edge *c = cg_.create_edge(top_,cg_.anchor).first;
	DDNS::NSd(c).minlen = DDNS::NSd(c).weight = 0;

	moveOldNodes(Q);
	Layout extraI(Q.whole);
	removeOldConstraints(Q,extraI);
	insertNewNodes(Q);
	insertNewEdges(Q.insE);
	insertNewEdges(extraI);
	for(typename Layout::graphedge_iter ei = extraI.edges().begin(); ei!=extraI.edges().end(); ++ei)
		ModifyEdge(Q,*ei,DG_UPD_MOVE);
	stabilizePositionedNodes(Q);
	recomputeRanks(Q);
	NextProcess(Q);
}

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRanker_h
