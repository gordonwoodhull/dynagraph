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
	NSRanker(ChangingGraph<Layout> *world) : 
		LinkedChangeProcessor<Layout>(world),
		top_(cg_.create_node())
	{}
	~NSRanker();
	void Process();
private:
	ConstraintGraph cg_;
	ConstraintGraph::Node *top_; // to pull loose nodes upward
	typedef FlexiRankXlator<Layout> RankXlator;
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
template<typename Layout>
NSRanker<Layout>::~NSRanker() {
	for(typename Layout::graphedge_iter i = this->world_->current_.edges().begin();i!=this->world_->current_.edges().end();++i)
		removePathConstraints(*i);
	for(typename Layout::node_iter j = this->world_->current_.nodes().begin(); j!=this->world_->current_.nodes().end(); ++j)
		removeLayoutNodeConstraints(*j);
}
template<typename Layout>
void NSRanker<Layout>::removeLayoutNodeConstraints(typename Layout::Node *n) {
	cg_.RemoveNodeConstraints(gd<NSRankerNode>(n).topC);
	cg_.RemoveNodeConstraints(gd<NSRankerNode>(n).bottomC);
}
template<typename Layout>
void NSRanker<Layout>::removePathConstraints(typename Layout::Edge *e) {
	if(gd<NSRankerEdge>(e).weak) {
		cg_.erase_node(gd<NSRankerEdge>(e).weak);
		gd<NSRankerEdge>(e).weak = 0;
	}
	if(gd<NSRankerEdge>(e).strong) {
		cg_.erase_edge(gd<NSRankerEdge>(e).strong);
		gd<NSRankerEdge>(e).strong = 0;
	}
	gd<EdgeGeom>(e).constraint = false;
}
template<typename Layout>
void NSRanker<Layout>::removeOldConstraints(ChangeQueue<Layout> &changeQ,Layout &extraI) {
	for(typename Layout::graphedge_iter ei = changeQ.delE.edges().begin(); ei!=changeQ.delE.edges().end();++ei) {
		removePathConstraints(*ei);
		typename Layout::Edge *e = *ei;
		if(typename Layout::Edge *e2 = this->world_->current_.find_edge(e->head,e->tail))
			if(assign(gd<NSRankerEdge>(e2).secondOfTwo,false)) 
				extraI.insert(e2);
	}
	for(typename Layout::node_iter ni = changeQ.delN.nodes().begin(); ni!=changeQ.delN.nodes().end();++ni) 
		removeLayoutNodeConstraints(*ni);
}
template<typename Layout>
void NSRanker<Layout>::makeStrongConstraint(typename Layout::Edge *e) {
	assert(!gd<NSRankerEdge>(e).strong);
	gd<EdgeGeom>(e).constraint = true;

	DDCGraph::Node *tvar = cg_.GetVar(gd<NSRankerNode>(e->tail).bottomC),
		*hvar = cg_.GetVar(gd<NSRankerNode>(e->head).topC);

	DDCGraph::Edge *constr = cg_.create_edge(tvar,hvar).first;
	gd<NSRankerEdge>(e).strong = constr;
	DDNS::NSE &nse = DDNS::NSd(constr);
	double length = std::max(0.,gd<EdgeGeom>(e).minLength);
	nse.minlen = RankXlator::HeightToDRank(&this->world_->whole_,length*gd<GraphGeom>(e->g).separation.y);
	nse.weight = EDGELENGTH_WEIGHT;
}
template<typename Layout>
void NSRanker<Layout>::makeWeakConstraint(typename Layout::Edge *e) {
	assert(!gd<NSRankerEdge>(e).weak);
	gd<EdgeGeom>(e).constraint = false;

	DDCGraph::Node *tvar = cg_.GetVar(gd<NSRankerNode>(e->tail).bottomC),
		*hvar = cg_.GetVar(gd<NSRankerNode>(e->head).topC);
	gd<NSRankerEdge>(e).weak = cg_.create_node();
	gd<ConstraintType>(gd<NSRankerEdge>(e).weak).why = ConstraintType::rankWeak;
	NSEdgePair ep(gd<NSRankerEdge>(e).weak,tvar,hvar);
	DDNS::NSd(ep.e[0]).minlen = 0;
	DDNS::NSd(ep.e[0]).weight = BACKEDGE_PENALTY;
	double length = std::max(0.,gd<EdgeGeom>(e).minLength);
	DDNS::NSd(ep.e[1]).minlen = RankXlator::HeightToDRank(&this->world_->whole_,length*gd<GraphGeom>(e->g).separation.y);
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
template<typename Layout>
void NSRanker<Layout>::doNodeHeight(typename Layout::Node *n) {
	DDCGraph::Node *tv = cg_.GetVar(gd<NSRankerNode>(n).topC),
		*bv = cg_.GetVar(gd<NSRankerNode>(n).bottomC);
	DDCGraph::Edge *heightC = cg_.create_edge(tv,bv).first;
	 // one-node chains cause trouble; make sure there's one edge
	DDNS::NSd(heightC).minlen = std::max(1,RankXlator::HeightToDRank(&this->world_->whole_,ROUND(gd<NodeGeom>(n).region.boundary.Height())));
	DDNS::NSd(heightC).weight = NODEHEIGHT_PENALTY;
}
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
template<typename Layout>
void NSRanker<Layout>::insertNewNodes(ChangeQueue<Layout> &changeQ) {
	for(typename Layout::node_iter ni = changeQ.insN.nodes().begin(); ni!=changeQ.insN.nodes().end(); ++ni) {
        typename Layout::Node *n = *ni;

		// pull loose nodes upward
		ConstraintGraph::Edge *pull = cg_.create_edge(top_,cg_.GetVar(gd<NSRankerNode>(n).topC)).first;
		DDNS::NSd(pull).minlen = 0;
		DDNS::NSd(pull).weight = UPWARD_TENDENCY;

		doNodeHeight(n);
	}
}
template<typename Layout>
void NSRanker<Layout>::stabilizePositionedNodes(ChangeQueue<Layout> &changeQ) {
	// all nodes with position should want to stay there
	// (this is particularly important for nodes that started without a position but got one last round)
	for(typename Layout::node_iter ni = changeQ.current->nodes().begin(); ni!=changeQ.current->nodes().end(); ++ni) {
        typename Layout::Node *n = *ni;
		if(!changeQ.delN.find(n)) {
			// stabilize at starting position if any
			if(gd<NodeGeom>(n).pos.valid) {
				int topRank = RankXlator::CoordToRank(changeQ.whole,gd<NodeGeom>(n).BoundingBox().t);
				cg_.Stabilize(gd<NSRankerNode>(n).topC,topRank,STABILITY_FACTOR_Y);
			}
			else
				cg_.Unstabilize(gd<NSRankerNode>(n).topC);
		}
	}
}
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
template<typename Layout>
void NSRanker<Layout>::insertNewEdges(Layout &insE) {
	for(typename Layout::graphedge_iter ei = insE.edges().begin(); ei!=insE.edges().end(); ++ei) {
		typename Layout::Edge *e = *ei;
		if(e->head == e->tail)
			continue;
		bool weak = false;
		if(typename Layout::Edge *e1 = this->world_->current_.find_edge(e->head,e->tail)) {
			// mark & ignore second leg of 2-cycle for all modeling purposes
			// DynaDAGServer will draw it by reversing the other
			// if both get inserted at once, mark the second processed here (should be 2nd inserted)
			if(gd<NSRankerEdge>(e1).weak || gd<NSRankerEdge>(e1).strong) {
				gd<NSRankerEdge>(e).secondOfTwo = true;
				continue;
			}
		}
		else if(pathExists<Layout>(this->world_->current_.find(e->head),this->world_->current_.find(e->tail)))
			weak = true;
		if(gd<NSRankerNode>(e->tail).rankFixed || gd<NSRankerNode>(e->head).rankFixed)
			weak = true;
		if(weak)
			makeWeakConstraint(e);
		else
			makeStrongConstraint(e);
		if(this->world_->current_.find(e->head)->degree()==1)
			cg_.Unstabilize(gd<NSRankerNode>(e->head).topC);
		if(this->world_->current_.find(e->tail)->degree()==1)
			cg_.Unstabilize(gd<NSRankerNode>(e->tail).topC);
	}
}
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
		assert(!RankXlator::Below(changeQ.whole,newTopRank,newBottomRank));
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
	this->NextProcess();
}

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRanker_h
