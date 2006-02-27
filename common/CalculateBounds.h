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

namespace Dynagraph {

template<typename Layout>
bool CalculateBounds(Layout *current) {
	Bounds &b = gd<GraphGeom>(current).bounds,
		b2;
	for(typename Layout::node_iter ni = current->nodes().begin(); ni!=current->nodes().end(); ++ni)
		b2 |= gd<NodeGeom>(*ni).BoundingBox();
	for(typename Layout::graphedge_iter ei = current->edges().begin(); ei!=current->edges().end(); ++ei)
		b2 |= gd<EdgeGeom>(*ei).pos.BoundingBox();
	if(b!=b2) {
		b = b2;
		return true;
	}
	else
		return false;
}

} // namespace Dynagraph
