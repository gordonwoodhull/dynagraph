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


#include "LayoutAttrs.h"
#include "Transform.h"
#include <sstream>
#include <iomanip>
using namespace std;

const unsigned int AllFlags = 0xffffffff;
// these write dynagraph Geom changes to the StrAttrs and StrAttrChanges attributes
DString g_NLPNames[5] = {"center","right","top","left","bottom"};


#include <sstream>
const double ln10 = 2.30258509299404568401799145468436;
inline void initStream(std::ostringstream &o,const Coord &resolution) {
	double lbase10 = log(resolution.x)/ln10;
	int precX = int(-floor(lbase10));
	o.flags(o.flags()|std::ios::fixed);
	o.width(0);
	o.precision(precX);
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
void stringifyBounds(const Bounds &b,Transform *trans,const Coord &res,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,res);
	if(b.valid && trans)
		outBounds(o,trans->out(b));
	else
		outBounds(o,b);
	out.put("bb",o.str());
}
void stringifyChangeRect(const Bounds &b,Transform *trans,const Coord &res,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,res);
	if(b.valid && trans)
		outBounds(o,trans->out(b));
	else
		outBounds(o,b);
	out.put("changerect",o.str());
}
void stringifyPos(const Position &p,Transform *trans,const Coord &res,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,res);
	if(p.valid) {
		Position p2 = trans?trans->out(p):p;
		o << p2.x << ',' << p2.y;
	}
	out.put("pos",o.str());
}
void stringifyPolyDef(const PolyDef &pdef,Transform *trans,const Coord &res,StrAttrs2 &out) {
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
void stringifyNodeLabels(const NodeLabels &nl,Transform *trans,const Coord &res,StrAttrs2 &out) {
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
			initStream(o,res);
			o << nl[i].size << ends;
			out.put(sname,o.str());
		}
		if(nl[i].bounds.valid) {
			ostringstream o;
			initStream(o,res);
			o << nl[i].bounds << ends;
			out.put(posname,o.str());
		}
	}
}
void stringifyEdgePos(const Line &ep,Transform *trans,const Coord &res,StrAttrs2 &out) {
	ostringstream o;
	initStream(o,res);
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
