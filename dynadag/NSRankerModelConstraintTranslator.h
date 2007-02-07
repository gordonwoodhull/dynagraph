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


#ifndef NSRankerModel_h
#define NSRankerModel_h

namespace Dynagraph {
namespace DynaDAG {

// Network Simplex is not an incremental algorithm -- yet.
// Fit it in the framework anyway: NSRankerModel changes are applied incrementally,
// but the return is handled exclusively by NSRankerModelToLayoutTranslator

struct NSRankerModelToConstraintTranslator : ChangeTranslator<NSRankerModel,LlelConstraintGraph> {
	typedef GoingQueueTransition<NSRankerModel,LlelConstraintGraph> Transition;
	NSRankerModelToConstraintTranslator(LlelConstraintGraph *whole,LlelConstraintGraph *current)
		: transition_(whole,current) {}
	void Process(ChangeProcessing *next) {
		ChangeQueue<Graph1> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<Graph2> &destQ = this->destWorld_->Q_;
        for(NSRankerModel::graphedge_iter ei = changeQ.delE.edges().begin(); ei!=changeQ.delE.edges().end();++ei)
            removeEdgeConstraints(*ei);

        for(NSRankerModel::node_iter ni = changeQ.delN.nodes().begin(); ni!=changeQ.delN.nodes().end();++ni)
            removeLayoutNodeConstraints(*ni);
	}
    void removeLayoutNodeConstraints(NSRankerModel::Node *n) {
        LlelConstraintGraph &cg = gd<NSRankerModelGraph>(n->g);
        NSRankerModelNode &rn = gd<NSRankerModelNode>(n);
        cg.RemoveNodeConstraints(rn.topC);
        cg.RemoveNodeConstraints(rn.bottomC);
    }
    void removeEdgeConstraints(NSRankerModel::Edge *e) {
        LlelConstraintGraph &cg = gd<NSRankerModelGraph>(e->g);
        NSRankerModelEdge &re = gd<NSRankerModelEdge>(e);
        if(re.weak) {
            cg.erase_node(re.weak);
            re.weak = 0;
        }
        if(re.strong) {
            cg.erase_edge(re.strong);
            re.strong = 0;
        }
        gd<EdgeGeom>(e).constraint = false;
    }
    void makeStrongConstraint(NSRankerModel::Edge *e) {
        LlelConstraintGraph &cg = gd<NSRankerModelGraph>(e->g);
        NSRankerModelEdge &re = gd<NSRankerModelEdge>(e);
        dgassert(!re.strong);

        LlelConstraintGraph::Node *tvar = cg.GetVar(gd<NSRankerNode>(e->tail).bottomC),
            *hvar = cg.GetVar(gd<NSRankerNode>(e->head).topC);

        LlelConstraintGraph::Edge *constr = cg.create_edge(tvar,hvar).first;
        re.strong = constr;
        LlelNS::NSE &nse = LlelNS::NSd(constr);
        double length = std::max(0.,gd<EdgeGeom>(e).minLength);
        nse.minlen = rankXlate_.HeightToDRank(length*gd<GraphGeom>(e->g).separation.y);
        nse.weight = EDGELENGTH_WEIGHT;
    }
    void makeWeakConstraint(NSRankerModel::Edge *e) {
        LlelConstraintGraph &cg = gd<NSRankerModelGraph>(e->g);
        NSRankerModelEdge &re = gd<NSRankerModelEdge>(e);
        dgassert(!re.weak);

        LlelConstraintGraph::Node *tvar = cg.GetVar(gd<NSRankerNode>(e->tail).bottomC),
            *hvar = cg.GetVar(gd<NSRankerNode>(e->head).topC);
        re.weak = cg.create_node();
        gd<ConstraintType>(re.weak).why = ConstraintType::rankWeak;
        NSEdgePair ep(re.weak,tvar,hvar);
        LlelNS::NSd(ep.e[0]).minlen = 0;
        LlelNS::NSd(ep.e[0]).weight = BACKEDGE_PENALTY;
        double length = std::max(0.,gd<EdgeGeom>(e).minLength);
        LlelNS::NSd(ep.e[1]).minlen = rankXlate_.HeightToDRank(length*gd<GraphGeom>(e->g).separation.y);
    }
    void NSRanker<Layout>::doNodeHeight(typename Layout::Node *n) {
        LlelConstraintGraph::Node *tv = cg_.GetVar(gd<NSRankerNode>(n).topC),
            *bv = cg_.GetVar(gd<NSRankerNode>(n).bottomC);
        LlelConstraintGraph::Edge *heightC = cg_.create_edge(tv,bv).first;
         // one-node chains cause trouble; make sure there's one edge
        LlelNS::NSd(heightC).minlen = std::max(1,rankXlate_.HeightToDRank(ROUND(gd<NodeGeom>(n).region.boundary.Height())));
        LlelNS::NSd(heightC).weight = NODEHEIGHT_PENALTY;
    }
    void NSRanker<Layout>::insertNewNodes(ChangeQueue<Layout> &changeQ) {
        for(typename Layout::node_iter ni = changeQ.insN.nodes().begin(); ni!=changeQ.insN.nodes().end(); ++ni) {
            typename Layout::Node *n = *ni;

            // pull loose nodes upward
            LlelConstraintGraph::Edge *pull = cg_.create_edge(top_,cg_.GetVar(gd<NSRankerNode>(n).topC)).first;
            LlelNS::NSd(pull).minlen = 0;
            LlelNS::NSd(pull).weight = UPWARD_TENDENCY;

            doNodeHeight(n);
        }
    }

};

// currently this is just to keep the processor chain alive
struct ConstraintToNSRankerModelTranslator : ChangeTranslator<LlelConstraintGraph,NSRankerModel> {
	typedef ReturningQueueTransition<LlelConstraintGraph,NSRankerModel> Transition;
	void Process(ChangeProcessing *next) {
		ChangeQueue<Graph1> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<Graph2> &destQ = this->destWorld_->Q_;
	}

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerModel_h
