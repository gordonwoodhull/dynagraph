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

#include "Transform.h"
#include "Geometry.h"
#include "ChangeQueue.h"

#ifndef stringsOut_h
#define stringsOut_h

namespace Dynagraph {

const unsigned int AllFlags = 0xffffffff;
extern DString g_NLPNames[5];

const double ln10 = 2.30258509299404568401799145468436;
extern void initStream(std::ostringstream &o,const Coord &resolution);

template<typename GO>
void stringifyDrawn(const Drawn &d,GO *go) {
	std::ostringstream stream;
	stream << d << std::ends;
	SetAndMark(go,"lines",stream.str());
}
inline void outBounds(std::ostream &o,const Bounds &b) {
	// dot rects are llx,lly,urx,ury
	if(b.valid)
		o << b;
	else
		o << "0,0,0,0";
}
template<typename GO>
void stringifyBounds(const Bounds &b,Transform *trans,const Coord &res,GO *go) {
	std::ostringstream o;
	initStream(o,res);
	if(b.valid && trans)
		outBounds(o,trans->out(b));
	else
		outBounds(o,b);
	SetAndMark(go,"bb",o.str());
}
template<typename GO>
void stringifyChangeRect(const Bounds &b,Transform *trans,const Coord &res,GO *go) {
	std::ostringstream o;
	initStream(o,res);
	if(b.valid && trans)
		outBounds(o,trans->out(b));
	else
		outBounds(o,b);
	SetAndMark(go,"changerect",o.str());
}
template<typename GO>
void stringifyPos(const Position &p,Transform *trans,const Coord &res,GO *go) {
	std::ostringstream o;
	initStream(o,res);
	if(p.valid) {
		Position p2 = trans?Position(trans->out(p)):p;
		o << p2.x << ',' << p2.y;
	}
	SetAndMark(go,"pos",o.str());
}
template<typename GO>
void stringifyPolyDef(const PolyDef &pdef,Transform *trans,const Coord &res,GO *go) {
	char buf[20]; 
	PolyDef norm;
	if(pdef.isEllipse) {
		if(!norm.isEllipse)
			SetAndMark(go,"shape","ellipse");
		if(pdef.aspect!=norm.aspect) {
			sprintf(buf,"%f",pdef.aspect);
			SetAndMark(go,"aspect",buf);
		}
	}
	else {
		StrAttrs &saout = gd<StrAttrs>(go);
		if(saout.find("shape")==saout.end())
			SetAndMark(go,"shape","polygon");
		if(pdef.sides!=norm.sides) {
			sprintf(buf,"%d",pdef.sides);
			SetAndMark(go,"sides",buf);
		}
	}
	if(pdef.peripheries!=norm.peripheries) {
		sprintf(buf,"%d",pdef.peripheries);
		SetAndMark(go,"peripheries",buf);
	}
	if(pdef.perispacing!=norm.perispacing) {
		sprintf(buf,"%f",pdef.perispacing);
		SetAndMark(go,"perispacing",buf);
	}
	if(pdef.rotation!=norm.rotation) {
		sprintf(buf,"%f",(pdef.rotation*180.0)/M_PI);
		SetAndMark(go,"orientation",buf);
	}
	if(pdef.skew!=norm.skew) {
		sprintf(buf,"%f",pdef.skew);
		SetAndMark(go,"skew",buf);
	}
	if(pdef.distortion!=norm.distortion) {
		sprintf(buf,"%f",pdef.distortion);
		SetAndMark(go,"distortion",buf);
	}
	if(pdef.regular)
		SetAndMark(go,"regular",0);
	if(pdef.exterior_box.x||pdef.exterior_box.y) {
		Coord conv = trans->outSize(pdef.exterior_box);
		if(conv.x) {
			sprintf(buf,"%f",conv.x);
			SetAndMark(go,"width",buf);
		}
		if(conv.y) {
			sprintf(buf,"%f",conv.y);
			SetAndMark(go,"height",buf);
		}
	}
}
template<typename GO>
void stringifyNodeLabels(const NodeLabels &nl,Transform *trans,const Coord &res,GO *go) {
	for(unsigned i = 0; i<nl.size(); ++i) {
		char plname[20] = "labelalign",
			sname[20] = "labelsize",
			posname[20] = "labelbounds";
		if(i>0) {
			sprintf(plname+strlen(plname),"%d",i);
			sprintf(sname+strlen(sname),"%d",i);
			sprintf(posname+strlen(posname),"%d",i);
		}
		SetAndMark(go,plname,g_NLPNames[nl[i].align]);
		{
			std::ostringstream o;
			initStream(o,res);
			o << nl[i].size << std::ends;
			SetAndMark(go,sname,o.str());
		}
		if(nl[i].bounds.valid) {
			std::ostringstream o;
			initStream(o,res);
			o << nl[i].bounds << std::ends;
			SetAndMark(go,posname,o.str());
		}
	}
}
template<typename GO>
void stringifyEdgePos(const Line &ep,Transform *trans,const Coord &res,GO *go) {
	std::ostringstream o;
	initStream(o,res);
	for(Line::const_iterator pi = ep.begin(); pi!=ep.end(); ++pi) {
		if(pi!=ep.begin())
			o << ' ';
		Coord p = *pi;
		if(trans)
			p = trans->out(p);
		o << p.x << ',' << p.y;
	}
    o << std::ends;
	SetAndMark(go,"pos",o.str());
}

// some attributes belong to all Geoms
template<typename GO>
void stringifyAny(Transform *trans,GO *go,Update u) {
	// if there's lines to tell OR there useta be and now there aren't...
	if(u.flags&DG_UPD_DRAWN && (gd<Drawn>(go).size() || gd<StrAttrs>(go)["lines"].length()))
		stringifyDrawn(gd<Drawn>(go),go);
}
template<typename Layout>
void stringsOut(Transform *trans,Layout *l,Update u) {
	stringifyAny(trans,l,u);
	GraphGeom &gg = gd<GraphGeom>(l);
	if(u.flags&DG_UPD_BOUNDS)
		stringifyBounds(gg.bounds,trans,gg.resolution,l);
	if(u.flags&DG_UPD_CHANGERECT)
		stringifyChangeRect(gg.changerect,trans,gg.resolution,l);
}
template<typename Layout>
void stringsOut(Transform *trans,typename Layout::Node *n,Update u) {
	stringifyAny(trans,n,u);
	NodeGeom &ng = gd<NodeGeom>(n);
	GraphGeom &gg = gd<GraphGeom>(n->g);
	if(u.flags&DG_UPD_MOVE)
		stringifyPos(ng.pos,trans,gg.resolution,n);
	if(u.flags&DG_UPD_POLYDEF)
		stringifyPolyDef(gd<PolyDef>(n),trans,gg.resolution,n);
	if(u.flags&DG_UPD_LABEL)
		stringifyNodeLabels(gd<NodeLabels>(n),trans,gg.resolution,n);
}
template<typename Layout>
void stringsOut(Transform *trans,typename Layout::Edge *e,Update u) {
	stringifyAny(trans,e,u);
	EdgeGeom &eg = gd<EdgeGeom>(e);
	GraphGeom &gg = gd<GraphGeom>(e->g);
	if(u.flags&DG_UPD_MOVE)
		stringifyEdgePos(eg.pos,trans,gg.resolution,e);
}
template<typename Layout>
void stringsOut(Transform *trans,ChangeQueue<Layout> &Q) {
	bool llchanged = false;
	if(trans) {
		/*
		// grappa used to require bounding box with lower left = (0,0)
		// this means you have to resend all coords pretty much every step
		// but there's probably a legitimate purpose for this disabled feature
		Coord ll = gd<GraphGeom>(Q.whole).bounds.LowerLeft();
		if(ll!=trans->ll) {
			trans->ll = ll;
			llchanged = true;
		}
		*/
	}
	if(ModifyFlags(Q).flags)
		stringsOut(trans,Q.ModGraph(),ModifyFlags(Q));
	typename Layout::node_iter ni;
	typename Layout::graphedge_iter ei;
	if(llchanged) {
		// not hit: see above
		// all coordinates have changed because they're based on lower-left corner
		for(ni = Q.current->nodes().begin(); ni!=Q.current->nodes().end(); ++ni)
			if(!Q.insN.find(*ni) && !Q.delN.find(*ni))
				stringsOut<Layout>(trans,*ni,DG_UPD_MOVE|igd<Update>(*ni).flags);
		for(ei = Q.current->edges().begin(); ei!=Q.current->edges().end(); ++ei)
			if(!Q.insE.find(*ei) && !Q.delE.find(*ei))
				stringsOut<Layout>(trans,*ei,DG_UPD_MOVE|igd<Update>(*ei).flags);
	}
	else {
		for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
			stringsOut<Layout>(trans,*ni,AllFlags);
		for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
			stringsOut<Layout>(trans,*ei,AllFlags);
		for(ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
			stringsOut<Layout>(trans,*ni,igd<Update>(*ni));
		for(ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
			stringsOut<Layout>(trans,*ei,igd<Update>(*ei));
	}
}

} // namespace Dynagraph

#endif // stringsOut_h
