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

#ifndef DynaDAGDefs_h
#define DynaDAGDefs_h

namespace Dynagraph {
namespace DynaDAG {

inline const char *type(DDModel::Node *mn) {
	return gd<DDNode>(mn).amEdgePart()?"path":"multinode";
}
inline void *thing(DDModel::Node *mn) {
	if(gd<DDNode>(mn).multi)
		return gd<DDNode>(mn).multi;
	else
		return gd<DDEdge>(*mn->ins().begin()).path;
}

typedef ChangeQueue<DynaDAGLayout> DDChangeQueue;

struct BadXConstraints : DGException {
  BadXConstraints() : DGException("an internal error has occurred in dynadag's x constraints",true) {}
};

inline bool userDefinedMove(DynaDAGLayout::Edge *ve) {
	return gd<EdgeGeom>(ve).manualRoute;
	//return flags & DG_UPD_MOVE && !gd<EdgeGeom>(ve).pos.Empty();
}

typedef std::vector<DynaDAGLayout::Node*> LNodeV; // mneh
// vertical constraint weights
#define UPWARD_TENDENCY 0 // try to pull children toward parents
#define	STABILITY_FACTOR_Y	1 // keep nodes near where they were
#define EDGELENGTH_WEIGHT 10 // try to shorten edges
#define	BACKEDGE_PENALTY	100  // try to point weak edges downward
#define NODEHEIGHT_PENALTY	1000000 // don't stretch nodes

// horizontal constraint weights
#define COMPACTION_STRENGTH 0 // how much to allow gaps between nodes
#define	STABILITY_FACTOR_X	100 // keep nodes near where they were
#define BEND_WEIGHT 1000 // keep adjacent v-nodes close

#define MINCROSS_PASSES 12
#define NODECROSS_PENALTY 1000 // don't let anything cross nodes

} // namespace DynaDAG
} // namespace Dynagraph

#endif // DynaDAGDefs_h
