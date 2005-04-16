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
#include "Transform.h"
#include <sstream>
#include <iomanip>
using namespace std;

const unsigned int AllFlags = 0xffffffff;
// these write dynagraph Geom changes to the StrAttrs and StrAttrChanges attributes
DString g_NLPNames[5] = {"center","right","top","left","bottom"};


#include <sstream>
const double ln10 = 2.30258509299404568401799145468436;
inline void initStream(std::ostringstream &o,Layout *l) {
	double lbase10 = log(gd<GraphGeom>(l).resolution.x)/ln10;
	int precX = int(-floor(lbase10));
	o.flags(o.flags()|std::ios::fixed);
	o.width(0);
	o.precision(precX);
}
// forward decls
void stringifyDrawn(const Drawn &d,StrAttrs2 &out);
void outBounds(std::ostream &o,const Bounds &b);
void stringifyBounds(const Bounds &b,Transform *trans,Layout *l,StrAttrs2 &out);
void stringifyChangeRect(const Bounds &b,Transform *trans,Layout *l,StrAttrs2 &out);
void stringifyPos(const Position &p,Transform *trans,Layout *l,StrAttrs2 &out);
void stringifyPolyDef(const PolyDef &pdef,Transform *trans,Layout *l,StrAttrs2 &out);
void stringifyNodeLabels(const NodeLabels &nl,Transform *trans,Layout *l,StrAttrs2 &out);
void stringifyEdgePos(const Line &ep,Transform *trans,Layout *l,StrAttrs2 &out);

// some attributes belong to all Geoms
template<typename T>
void stringifyAny(Transform *trans,T *o,Update u) {
	// if there's lines to tell OR there useta be and now there aren't...
	if(u.flags&DG_UPD_DRAWN && (gd<Drawn>(o).size() || gd<StrAttrs>(o)["lines"].length()))
		stringifyDrawn(gd<Drawn>(o),gd<StrAttrs2>(o));
}
void stringifyChanges(Transform *trans,Layout *l,Update u) {
	stringifyAny(trans,l,u);
	GraphGeom &gg = gd<GraphGeom>(l);
	StrAttrs2 &att = gd<StrAttrs2>(l);
	if(u.flags&DG_UPD_BOUNDS)
		stringifyBounds(gg.bounds,trans,l,att);
	if(u.flags&DG_UPD_CHANGERECT)
		stringifyChangeRect(gg.changerect,trans,l,att);
}
void stringifyChanges(Transform *trans,Layout::Node *n,Update u) {
	stringifyAny(trans,n,u);
	NodeGeom &ng = gd<NodeGeom>(n);
	StrAttrs2 &att = gd<StrAttrs2>(n);
	if(u.flags&DG_UPD_MOVE)
		stringifyPos(ng.pos,trans,n->g,att);
	if(u.flags&DG_UPD_POLYDEF)
		stringifyPolyDef(gd<PolyDef>(n),trans,n->g,att);
	if(u.flags&DG_UPD_LABEL)
		stringifyNodeLabels(gd<NodeLabels>(n),trans,n->g,att);
}
void stringifyChanges(Transform *trans,Layout::Edge *e,Update u) {
	stringifyAny(trans,e,u);
	EdgeGeom &eg = gd<EdgeGeom>(e);
	StrAttrs2 &att = gd<StrAttrs2>(e);
	if(u.flags&DG_UPD_MOVE)
		stringifyEdgePos(eg.pos,trans,e->g,att);
}
void stringsOut(Transform *trans,ChangeQueue &Q) {
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
		stringifyChanges(trans,Q.client,Q.GraphUpdateFlags());
	Layout::node_iter ni;
	Layout::graphedge_iter ei;
	if(llchanged) {
		// all coordinates have changed because they're based on lower-left corner
		for(ni = Q.current->nodes().begin(); ni!=Q.current->nodes().end(); ++ni)
			if(!Q.insN.find(*ni) && !Q.delN.find(*ni))
				stringifyChanges(trans,*ni,DG_UPD_MOVE|igd<Update>(*ni).flags);
		for(ei = Q.current->edges().begin(); ei!=Q.current->edges().end(); ++ei)
			if(!Q.insE.find(*ei) && !Q.delE.find(*ei))
				stringifyChanges(trans,*ei,DG_UPD_MOVE|igd<Update>(*ei).flags);
	}
	else {
		for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
			stringifyChanges(trans,*ni,AllFlags);
		for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
			stringifyChanges(trans,*ei,AllFlags);
		for(ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
			stringifyChanges(trans,*ni,igd<Update>(*ni));
		for(ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
			stringifyChanges(trans,*ei,igd<Update>(*ei));
	}
}


void stringifyDrawn(const Drawn &d,StrAttrs2 &out) {
	ostringstream stream;
	stream << d << ends;
	out.put("lines",stream.str());
}
void outBounds(ostream &o,const Bounds &b) {
	// dot rects are llx,lly,urx,ury
	if(b.valid)
		o << b;
	else
		o << "0,0,0,0";
}
void stringifyBounds(const Bounds &b,Transform *trans,Layout *l,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,l);
	if(b.valid && trans)
		outBounds(o,trans->out(b));
	else
		outBounds(o,b);
	out.put("bb",o.str());
}
void stringifyChangeRect(const Bounds &b,Transform *trans,Layout *l,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,l);
	if(b.valid && trans)
		outBounds(o,trans->out(b));
	else
		outBounds(o,b);
	out.put("changerect",o.str());
}
void stringifyPos(const Position &p,Transform *trans,Layout *l,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,l);
	if(p.valid) {
		Position p2 = trans?Position(trans->out(p)):p;
		o << p2.x << ',' << p2.y;
	}
	out.put("pos",o.str());
}
void stringifyPolyDef(const PolyDef &pdef,Transform *trans,Layout *l,StrAttrs2 &out) {
	char buf[20]; // agh  can't make myself use <<, am i getting old?
	PolyDef norm;
	if(pdef.isEllipse) {
		if(!norm.isEllipse)
			out.put("shape","ellipse");
		if(pdef.aspect!=norm.aspect) {
			sprintf(buf,"%f",pdef.aspect);
			out.put("aspect",buf);
		}
	}
	else {
		StrAttrs &saout = out;
		if(saout.find("shape")==saout.end())
			out.put("shape","polygon");
		if(pdef.sides!=norm.sides) {
			sprintf(buf,"%d",pdef.sides);
			out.put("sides",buf);
		}
	}
	if(pdef.peripheries!=norm.peripheries) {
		sprintf(buf,"%d",pdef.peripheries);
		out.put("peripheries",buf);
	}
	if(pdef.perispacing!=norm.perispacing) {
		sprintf(buf,"%f",pdef.perispacing);
		out.put("perispacing",buf);
	}
	if(pdef.rotation!=norm.rotation) {
		sprintf(buf,"%f",(pdef.rotation*180.0)/M_PI);
		out.put("orientation",buf);
	}
	if(pdef.skew!=norm.skew) {
		sprintf(buf,"%f",pdef.skew);
		out.put("skew",buf);
	}
	if(pdef.distortion!=norm.distortion) {
		sprintf(buf,"%f",pdef.distortion);
		out.put("distortion",buf);
	}
	if(pdef.regular)
		out.put("regular",0);
	if(pdef.exterior_box.x||pdef.exterior_box.y) {
		Coord conv = trans->outSize(pdef.exterior_box);
		if(conv.x) {
			sprintf(buf,"%f",conv.x);
			out.put("width",buf);
		}
		if(conv.y) {
			sprintf(buf,"%f",conv.y);
			out.put("height",buf);
		}
	}
}
void stringifyNodeLabels(const NodeLabels &nl,Transform *trans,Layout *l,StrAttrs2 &out) {
	for(unsigned i = 0; i<nl.size(); ++i) {
		char plname[20] = "labelalign",
			sname[20] = "labelsize",
			posname[20] = "labelbounds";
		if(i>0) {
			sprintf(plname+strlen(plname),"%d",i);
			sprintf(sname+strlen(sname),"%d",i);
			sprintf(posname+strlen(posname),"%d",i);
		}
		out.put(plname,g_NLPNames[nl[i].align]);
		{
			ostringstream o;
			initStream(o,l);
			o << nl[i].size << ends;
			out.put(sname,o.str());
		}
		if(nl[i].bounds.valid) {
			ostringstream o;
			initStream(o,l);
			o << nl[i].bounds << ends;
			out.put(posname,o.str());
		}
	}
}
void stringifyEdgePos(const Line &ep,Transform *trans,Layout *l,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,l);
	for(Line::const_iterator pi = ep.begin(); pi!=ep.end(); ++pi) {
		if(pi!=ep.begin())
			o << ' ';
		Coord p = *pi;
		if(trans)
			p = trans->out(p);
		o << p.x << ',' << p.y;
	}
    o << ends;
	out.put("pos",o.str());
}
