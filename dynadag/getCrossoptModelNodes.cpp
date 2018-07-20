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

namespace Dynagraph {
namespace DynaDAG {

void getCrossoptModelNodes(DynaDAGLayout &nodes,DynaDAGLayout &edges,NodeV &out) {
    for(DynaDAGLayout::node_iter ni = nodes.nodes().begin(); ni!=nodes.nodes().end(); ++ni)
        for(DDMultiNode::node_iter mnni = DDp(*ni)->nBegin(); mnni!=DDp(*ni)->nEnd(); ++ mnni)
            out.push_back(*mnni);
    int ec=0;
    for(DynaDAGLayout::graphedge_iter ei = edges.edges().begin(); ei!=edges.edges().end(); ++ei,++ec)
        for(DDPath::node_iter pni = DDp(*ei)->nBegin(); pni!=DDp(*ei)->nEnd(); ++ pni)
            out.push_back(*pni);
    loops.Field(dgr::crossopt,"layout nodes for crossopt",nodes.nodes().size());
    loops.Field(dgr::crossopt,"layout edges for crossopt",ec);

}

} // namespace DynaDAG
} // namespace Dynagraph

