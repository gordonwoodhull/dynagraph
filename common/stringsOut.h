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

#ifndef stringsOut_h
#define stringsOut_h

namespace Dynagraph {

const unsigned int AllFlags = 0xffffffff;

void stringifyDrawn(const Drawn &d,StrAttrs2 &out);
void outBounds(std::ostream &o,const Bounds &b);
void stringifyBounds(const Bounds &b,Transform *trans,const Coord &res,StrAttrs2 &out);
void stringifyChangeRect(const Bounds &b,Transform *trans,const Coord &res,StrAttrs2 &out);
void stringifyPos(const Position &p,Transform *trans,const Coord &res,StrAttrs2 &out);
void stringifyPolyDef(const PolyDef &pdef,Transform *trans,const Coord &res,StrAttrs2 &out);
void stringifyNodeLabels(const NodeLabels &nl,Transform *trans,const Coord &res,StrAttrs2 &out);
void stringifyEdgePos(const Line &ep,Transform *trans,const Coord &res,StrAttrs2 &out);

// some attributes belong to all Geoms
template<typename T>
void stringifyAny(Transform *trans,T *o,Update u) {
	// if there's lines to tell OR there useta be and now there aren't...
	if(u.flags&DG_UPD_DRAWN && (gd<Drawn>(o).size() || gd<StrAttrs>(o)["lines"].length()))
		stringifyDrawn(gd<Drawn>(o),gd<StrAttrs2>(o));
}
template<typename Layout>
void stringifyLayoutChanges(Transform *trans,Layout *l,Update u) {
	stringifyAny(trans,l,u);
	GraphGeom &gg = gd<GraphGeom>(l);
	StrAttrs2 &att = gd<StrAttrs2>(l);
	if(u.flags&DG_UPD_BOUNDS)
		stringifyBounds(gg.bounds,trans,gg.resolution,att);
	if(u.flags&DG_UPD_CHANGERECT)
		stringifyChangeRect(gg.changerect,trans,gg.resolution,att);
}
template<typename Layout>
void stringifyNodeChanges(Transform *trans,typename Layout::Node *n,Update u) {
	stringifyAny(trans,n,u);
	NodeGeom &ng = gd<NodeGeom>(n);
	GraphGeom &gg = gd<GraphGeom>(n->g);
	StrAttrs2 &att = gd<StrAttrs2>(n);
	if(u.flags&DG_UPD_MOVE)
		stringifyPos(ng.pos,trans,gg.resolution,att);
	if(u.flags&DG_UPD_POLYDEF)
		stringifyPolyDef(gd<PolyDef>(n),trans,gg.resolution,att);
	if(u.flags&DG_UPD_LABEL)
		stringifyNodeLabels(gd<NodeLabels>(n),trans,gg.resolution,att);
}
template<typename Layout>
void stringifyEdgeChanges(Transform *trans,typename Layout::Edge *e,Update u) {
	stringifyAny(trans,e,u);
	EdgeGeom &eg = gd<EdgeGeom>(e);
	GraphGeom &gg = gd<GraphGeom>(e->g);
	StrAttrs2 &att = gd<StrAttrs2>(e);
	if(u.flags&DG_UPD_MOVE)
		stringifyEdgePos(eg.pos,trans,gg.resolution,att);
}
template<typename Layout>
void stringsOut(Transform *trans,ChangeQueue<Layout> &Q) {
	bool llchanged = false;
	if(trans) {
		/*
		// grappa used to require bounding box with lower left = (0,0)
		// this means you have to resend all coords pretty much every step
		// but there's probably a legitimate purpose for this disabled feature
		Coord ll = gd<GraphGeom>(Q.client).bounds.LowerLeft();
		if(ll!=trans->ll) {
			trans->ll = ll;
			llchanged = true;
		}
		*/
	}
	if(Q.GraphUpdateFlags())
		stringifyLayoutChanges(trans,Q.client,Q.GraphUpdateFlags());
	typename Layout::node_iter ni;
	typename Layout::graphedge_iter ei;
	if(llchanged) {
		// all coordinates have changed because they're based on lower-left corner
		for(ni = Q.current->nodes().begin(); ni!=Q.current->nodes().end(); ++ni)
			if(!Q.insN.find(*ni) && !Q.delN.find(*ni))
				stringifyNodeChanges<Layout>(trans,*ni,DG_UPD_MOVE|igd<Update>(*ni).flags);
		for(ei = Q.current->edges().begin(); ei!=Q.current->edges().end(); ++ei)
			if(!Q.insE.find(*ei) && !Q.delE.find(*ei))
				stringifyEdgeChanges<Layout>(trans,*ei,DG_UPD_MOVE|igd<Update>(*ei).flags);
	}
	else {
		for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
			stringifyNodeChanges<Layout>(trans,*ni,AllFlags);
		for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
			stringifyEdgeChanges<Layout>(trans,*ei,AllFlags);
		for(ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
			stringifyNodeChanges<Layout>(trans,*ni,igd<Update>(*ni));
		for(ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
			stringifyEdgeChanges<Layout>(trans,*ei,igd<Update>(*ei));
	}
}

} // namespace Dynagraph

#endif // stringsOut_h
