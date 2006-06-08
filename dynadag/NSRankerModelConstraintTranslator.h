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

struct NSRankerModelToConstraintTranslator : ChangeTranslator<NSRankerModel,ConstraintGraph> {
	typedef GoingQueueTransition<NSRankerModel,ConstraintGraph> Transition;
	NSRankerModelToConstraintTranslator(ConstraintGraph *whole,ConstraintGraph *current)
		: transition_(whole,current) {}
	void Process() {
		ChangeQueue<Graph1> &Q1 = LinkedChangeProcessor<Graph1>::world_->Q_;
		ChangeQueue<Graph2> &Q2 = LinkedChangeProcessor<Graph2>::world_->Q_;
        for(NSRankerModel::graphedge_iter ei = changeQ.delE.edges().begin(); ei!=changeQ.delE.edges().end();++ei)
            removePathConstraints(*ei);

        for(NSRankerModel::node_iter ni = changeQ.delN.nodes().begin(); ni!=changeQ.delN.nodes().end();++ni)
            removeLayoutNodeConstraints(*ni);

		Transition::EndLastQ(Q1);
		LinkedChangeProcessor<Graph2>::NextProcess();
	}
    void removeLayoutNodeConstraints(NSRankerModel::Node *n) {
        ConstraintGraph &cg = gd<NSRankerModelGraph>(n->g);
        NSRankerModelNode &rn = gd<NSRankerModelNode>(n);
        cg.RemoveNodeConstraints(rn.topC);
        cg.RemoveNodeConstraints(rn.bottomC);
    }
    void removePathConstraints(NSRankerModel::Edge *e) {
        ConstraintGraph &cg = gd<NSRankerModelGraph>(e->g);
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
        ConstraintGraph &cg = gd<NSRankerModelGraph>(e->g);
        NSRankerModelEdge &re = gd<NSRankerModelEdge>(e);
        assert(!re.strong);

        DDCGraph::Node *tvar = cg.GetVar(gd<NSRankerNode>(e->tail).bottomC),
            *hvar = cg.GetVar(gd<NSRankerNode>(e->head).topC);

        DDCGraph::Edge *constr = cg.create_edge(tvar,hvar).first;
        re.strong = constr;
        DDNS::NSE &nse = DDNS::NSd(constr);
        double length = std::max(0.,gd<EdgeGeom>(e).minLength);
        nse.minlen = rankXlate_.HeightToDRank(length*gd<GraphGeom>(e->g).separation.y);
        nse.weight = EDGELENGTH_WEIGHT;
    }
    void makeWeakConstraint(NSRankerModel::Edge *e) {
        ConstraintGraph &cg = gd<NSRankerModelGraph>(e->g);
        NSRankerModelEdge &re = gd<NSRankerModelEdge>(e);
        assert(!re.weak);

        DDCGraph::Node *tvar = cg.GetVar(gd<NSRankerNode>(e->tail).bottomC),
            *hvar = cg.GetVar(gd<NSRankerNode>(e->head).topC);
        re.weak = cg.create_node();
        gd<ConstraintType>(re.weak).why = ConstraintType::rankWeak;
        NSEdgePair ep(re.weak,tvar,hvar);
        DDNS::NSd(ep.e[0]).minlen = 0;
        DDNS::NSd(ep.e[0]).weight = BACKEDGE_PENALTY;
        double length = std::max(0.,gd<EdgeGeom>(e).minLength);
        DDNS::NSd(ep.e[1]).minlen = rankXlate_.HeightToDRank(length*gd<GraphGeom>(e->g).separation.y);
    }
    void NSRanker<Layout>::doNodeHeight(typename Layout::Node *n) {
        DDCGraph::Node *tv = cg_.GetVar(gd<NSRankerNode>(n).topC),
            *bv = cg_.GetVar(gd<NSRankerNode>(n).bottomC);
        DDCGraph::Edge *heightC = cg_.create_edge(tv,bv).first;
         // one-node chains cause trouble; make sure there's one edge
        DDNS::NSd(heightC).minlen = std::max(1,rankXlate_.HeightToDRank(ROUND(gd<NodeGeom>(n).region.boundary.Height())));
        DDNS::NSd(heightC).weight = NODEHEIGHT_PENALTY;
    }
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

};

// currently this is just to keep the processor chain alive
struct ConstraintToNSRankerModelTranslator : ChangeTranslator<ConstraintGraph,NSRankerModel> {
	typedef ReturningQueueTransition<ConstraintGraph,NSRankerModel> Transition;
	void Process() {
		ChangeQueue<Graph1> &Q1 = LinkedChangeProcessor<Graph1>::world_->Q_;
		ChangeQueue<Graph2> &Q2 = LinkedChangeProcessor<Graph2>::world_->Q_;
		Transition::EndLastQ(Q1);
		LinkedChangeProcessor<Graph2>::NextProcess(Q2);
	}

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerModel_h
