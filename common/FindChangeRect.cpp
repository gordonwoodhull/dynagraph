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


#include "Dynagraph.h"
#include "FindChangeRect.h"
void FCRBefore::Process(ChangeQueue &Q) {
	data->changeN = Q.insN|Q.modN|Q.delN;
	data->changeE = Q.insE|Q.modE|Q.delE;
}
void FCRAfter::Process(ChangeQueue &Q) {
	Bounds changerect;
	for(Layout::node_iter ni = data->changeN.nodes().begin(); ni!=data->changeN.nodes().end(); ++ni)
		changerect |= gd<NodeGeom>(*ni).BoundingBox();
	for(Layout::graphedge_iter ei = data->changeE.edges().begin(); ei!=data->changeE.edges().end(); ++ei)
		changerect |= gd<EdgeGeom>(*ei).pos.BoundingBox();
	if(assign(gd<GraphGeom>(Q.current).changerect,changerect))
		Q.GraphUpdateFlags() |= DG_UPD_CHANGERECT;
}
