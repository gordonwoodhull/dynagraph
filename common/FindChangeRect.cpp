/*   Copyright (c) AT&T Corp.  All rights reserved.
   
This software may only be used by you under license from 
AT&T Corp. ("AT&T").  A copy of AT&T's Source Code Agreement 
is available at AT&T's Internet website having the URL 

http://www.research.att.com/sw/tools/graphviz/license/

If you received this software without first entering into a license 
with AT&T, you have an infringing copy of this software and cannot 
use it without violating AT&T's intellectual property rights. */

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
