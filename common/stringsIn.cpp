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
#include "stringsIn.h"
#include "genpoly.h"
#include <sstream>
#include "attrvalues.h"

using namespace std;

bool attrChanged(const StrAttrs &oldAttrs,const StrAttrs &newAttrs, const DString &name) {
	return newAttrs.find(name)!=newAttrs.end();
  /*
   alas, the attribute can have been changed already but not yet processed by shapegen
  StrAttrs::const_iterator ai1,ai2;
  if((ai2 = newAttrs.find(name))==newAttrs.end())
    return false;
  if((ai1 = oldAttrs.find(name))==oldAttrs.end())
    return true;
	return ai1->second!=ai2->second;
	*/
}
bool transformShape(Transform *trans,Line &shape) {
	bool nonzero = false;
	for(Line::iterator pi = shape.begin(); pi!=shape.end(); ++pi) {
		if(pi->x!=0 || pi->y!=0)
			nonzero = true;
		*pi = trans->in(*pi);
	}
	return nonzero;
}
// when the clearOld flag is set, every attribute that existed before but is
// not in the apply set should be set = "".  this adds those commands.
void clearRemoved(const StrAttrs &current,const StrAttrs &apply,StrAttrs &ret) {
    ret = apply;
	for(StrAttrs::const_iterator ai = current.begin(); ai!=current.end(); ++ai)
        ret[ai->first]; // referencing will create w value blank if wasn't in apply
}
void ensureAttr(const StrAttrs &att,StrAttrs &A, DString name) {
    // make sure that some attributes don't get left unset
    // by setting them to "" if they're not there
    if(att.find(name)==att.end())
        A[name]; // lookup creates blank entry if not there
}
Update stringsIn(Transform *trans,bool useDotDefaults,Layout *l,const StrAttrs &attrs,bool clearOld) {
	StrAttrs allChanges = attrs;
	if(clearOld)
		clearRemoved(gd<StrAttrs>(l),attrs,allChanges);
    StrAttrs2 &att = gd<StrAttrs2>(l);
    ensureAttr(att,allChanges,"resolution");
    ensureAttr(att,allChanges,"separation");
    ensureAttr(att,allChanges,"defaultsize");
	for(StrAttrs::const_iterator ai = allChanges.begin(); ai!=allChanges.end(); ++ai) {
        DString name = ai->first,
            value = ai->second;
		if(name=="resolution") {
			if(value.empty())
                value = useDotDefaults?"1,5":"0.1,0.1";
			istringstream s(value);
			s >> gd<GraphGeom>(l).resolution;

		}
		else if(name=="separation") {
			if(value.empty())
                value = useDotDefaults?"24,24":"0.5,0.5";
			istringstream s(value);
			s >> gd<GraphGeom>(l).separation;
		}
		else if(name=="defaultsize") {
			if(value.empty())
                value = useDotDefaults?"54,36":"1.5,1";
			istringstream s(value);
			s >> gd<GraphGeom>(l).defaultSize;
		}
		else if(name=="ticks") {
			if(value.empty())
                value = "0";
			istringstream s(value);
			s >> gd<GraphGeom>(l).ticks;
		}
		att.put(ai->first,value);
	}
	return 0;
}
// if any of these attrs get changed it's time to recalc shape
const DString g_shapeAttrs[] = {
	"boundary",
	"shape",
	"sides",
	"peripheries",
	"perispacing",
	"orientation",
	"regular",
	"skew",
	"distortion",
	"labelsize"
};
const int g_numShapeAttrs = sizeof(g_shapeAttrs)/sizeof(DString);
bool shapeChanged(const StrAttrs &oldAttrs,const StrAttrs &newAttrs) {
	for(int i = 0; i<g_numShapeAttrs; ++i)
		if(attrChanged(oldAttrs,newAttrs,g_shapeAttrs[i]))
			return true;
	return false;
}
const Coord house_coords[6] = {Coord(1,-1),Coord(1,1),Coord(0,2),Coord(-1,1),Coord(-1,-1),Coord(1,-1)};
const Coord trapezium_coords[5] = {Coord(2,-1),Coord(1,1),Coord(-1,1),Coord(-2,-1),Coord(2,-1)};
PolyDef readPolyDef(Transform *trans,StrAttrs &attrs) {
	PolyDef ret;
	StrAttrs::iterator ai;
	if((ai = attrs.find("shape"))!=attrs.end()) {
        assert(!ai->second.empty()); // this must be set up by assureattrs
		if(ai->second=="ellipse")
			ret.isEllipse = true;
		else if(ai->second=="hexagon") {
			ret.regular = true;
			ret.sides = 6;
		}
		else if(ai->second=="box")
			; // default
		else if(ai->second=="circle") {
			ret.regular = true;
			ret.isEllipse = true;
		}
		else if(ai->second=="diamond") {
			ret.rotation = M_PI/4;
		}
		else if(ai->second=="doublecircle") {
			ret.isEllipse = true;
			ret.regular = true;
			ret.peripheries = 1;
		}
		else if(ai->second=="doubleoctagon") {
			ret.sides = 8;
			ret.peripheries = 1;
		}
		else if(ai->second=="egg") {
			ret.isEllipse = true;
			ret.distortion = 1.3;
			ret.rotation = M_PI_2;
		}
		else if(ai->second=="hexagon") {
			ret.sides = 6;
		}
		else if(ai->second=="house") {
			ret.input.degree = 1;
			ret.input.insert(ret.input.begin(),house_coords,house_coords+6);
		}
		else if(ai->second=="invhouse") {
			ret.input.degree = 1;
			ret.input.insert(ret.input.begin(),house_coords,house_coords+6);
			ret.rotation = M_PI;
		}
		else if(ai->second=="invtrapezium") {
			ret.input.degree = 1;
			ret.input.insert(ret.input.begin(),trapezium_coords,trapezium_coords+5);
			ret.rotation = M_PI;
		}
		else if(ai->second=="invtriangle") {
			ret.sides = 3;
			ret.rotation = M_PI;
		}
		else if(ai->second=="octagon") {
			ret.sides = 8;
		}
		else if(ai->second=="parallelogram") {
			ret.sides = 4;
			ret.skew = 0.5;
		}
		else if(ai->second=="trapezium") {
			ret.input.degree = 1;
			ret.input.insert(ret.input.begin(),trapezium_coords,trapezium_coords+5);
		}
		else if(ai->second=="triangle") {
			ret.sides = 3;
		}
		else if(ai->second=="tripleoctagon") {
			ret.sides = 8;
			ret.peripheries = 2;
		}
	}
    if((ai = attrs.find("regular"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "false";
		ret.regular = ai->second=="true";
    }
	if((ai = attrs.find("sides"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "4";
        istringstream stream(ai->second);
	    stream >> ret.sides;
        if(ret.sides==0) //?
            attrs["shape"] = "plaintext";
	}
	if((ai = attrs.find("peripheries"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "0";
		istringstream stream(ai->second);
		stream >> ret.peripheries;
	}
	if((ai = attrs.find("perispacing"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "0";
		istringstream stream(ai->second);
		stream >> ret.perispacing;
	}
	if((ai = attrs.find("orientation"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "0";
		istringstream stream(ai->second);
		double degrees;
		stream >> degrees;
		ret.rotation = (degrees*M_PI)/180.0;
	}
	if((ai = attrs.find("skew"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "0";
		istringstream stream(ai->second);
		stream >> ret.skew;
	}
	if((ai = attrs.find("distortion"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "0";
		istringstream stream(ai->second);
		stream >> ret.distortion;
	}
	if((ai = attrs.find("labelsize"))!=attrs.end()) {
        if(ai->second.empty())
            ai->second = "0,0";
		istringstream stream(ai->second);
		stream >> ret.interior_box;
	}
	if((ai = attrs.find("width"))!=attrs.end()) {
        assert(!ai->second.empty()); // this must be set up by assureattrs
		istringstream stream(ai->second);
		stream >> ret.exterior_box.x;
	}
	if((ai = attrs.find("height"))!=attrs.end()) {
        assert(!ai->second.empty()); // this must be set up by assureattrs
		istringstream stream(ai->second);
		stream >> ret.exterior_box.y;
	}
	ret.interior_box = trans->inSize(ret.interior_box);
	ret.exterior_box = trans->inSize(ret.exterior_box);
	return ret;
}
Update assureAttrs(Transform *trans,Layout::Node *n) {
	Update ret;
	StrAttrs &att = gd<StrAttrs>(n);
	StrAttrs2 &att2 = gd<StrAttrs2>(n);
	if(att["shape"].empty()) {
        DString value = (att.find("sides")!=att.end())?"polygon":"ellipse";
        if(att2.put("shape",value))
		    ret.flags |= DG_UPD_REGION;
	}
	StrAttrs::iterator ai;
	Coord size(0,0);
    if((ai=att.find("width"))!=att.end() && !ai->second.empty())
		sscanf(ai->second.c_str(),"%lf",&size.x);
	if((ai=att.find("height"))!=att.end() && !ai->second.empty())
		sscanf(ai->second.c_str(),"%lf",&size.y);
	if(size.x&&size.y)
		return ret;
	if(size.x||size.y) {
		if(!size.y)
			size.y = size.x;
		if(!size.x)
			size.x = size.y;
	}
	else
		size = trans->outSize(gd<GraphGeom>(n->g).defaultSize);
	char buf[20];
	sprintf(buf,"%f",size.x);
	bool ch = att2.put("width",buf);
	sprintf(buf,"%f",size.y);
	ch |= att2.put("height",buf);
    if(ch)
	    ret.flags |= DG_UPD_REGION;
	return ret;
}
int ds2int(const DString &s) {
	int i = 0;
	for(unsigned j=0; j<s.length(); ++j)
		if(!isdigit(s[j]))
			return -1;
		else
			i = i*10 + s[j]-'0';
	return i;
}
Update stringsIn(Transform *trans,Layout::Node *n,const StrAttrs &attrs,bool clearOld) {
	StrAttrs allChanges;
	if(clearOld)
		clearRemoved(gd<StrAttrs>(n),attrs,allChanges);
	const StrAttrs &A = clearOld?allChanges:attrs;
	Update ret;
	NodeGeom &ng = gd<NodeGeom>(n);
	StrAttrs &att = gd<StrAttrs>(n);
	StrAttrs::const_iterator ai;
	bool chshape = shapeChanged(att,A);
	if(chshape)
		ret.flags |= DG_UPD_POLYDEF;
	for(ai = A.begin(); ai!=A.end(); ++ai) {
		if(ai->first=="pos") {
            if(ai->second.empty())  // position intentionally removed
                ng.pos.valid = false;
            else {
			    ng.pos.valid = true;
			    istringstream stream(ai->second);
			    stream >> ng.pos;
			    ng.pos = trans->in(ng.pos);
            }
			ret.flags |= DG_UPD_MOVE;
		}
		else if(ai->first.compare(0,9,"labelsize")==0) {
			int i=ds2int(ai->first.substr(9));
			if(i>=0) {
				istringstream stream(ai->second);
				stream >> gd<NodeLabels>(n)[i].size;
				ret.flags |= DG_UPD_LABEL;
			}
		}
		else if(ai->first.compare(0,14,"labelalign")==0) {
			int i=ds2int(ai->first.substr(14));
			if(i>=0) {
				gd<NodeLabels>(n)[i].align = string2nlp(ai->second);
				ret.flags |= DG_UPD_LABEL;
			}
		}
		else if(ai->first.compare(0,2,"labelbounds")==0) {
			int i=ds2int(ai->first.substr(9));
			if(i>=0) {
				istringstream stream(ai->second);
				stream >> gd<NodeLabels>(n)[i].bounds;
				ret.flags |= DG_UPD_LABEL;
			}
		}
		gd<StrAttrs2>(n).put(ai->first,ai->second);
	}
	ret.flags |= assureAttrs(trans,n).flags;
	if(ret.flags&(DG_UPD_REGION|DG_UPD_POLYDEF)) {
		if((ai = att.find("boundary"))!=att.end() && !ai->second.empty()) {
			const DString &s = ai->second;
			istringstream stream(s);
			ng.region.shape.Clear();
			stream >> ng.region.shape;
			ng.region.updateBounds();
			gd<Drawn>(n).clear();
			gd<Drawn>(n).push_back(ng.region.shape);
			gd<IfPolyDef>(n).whether = false;
			ret.flags |= DG_UPD_REGION|DG_UPD_DRAWN;
		}
		else if((ai = att.find("shape"))!=att.end() && ai->second=="plaintext") {
			Coord size;
			ai = att.find("labelsize");
			if(ai != att.end()) {
				istringstream stream(ai->second);
				stream >> size;
			}
			size = trans->inSize(size);
			gd<Drawn>(n).clear();
			ng.region.shape.Clear();
			ng.region.boundary = Rect(-size.x/2,size.y/2,size.x/2,-size.y/2);
			gd<IfPolyDef>(n).whether = false;
			ret.flags |= DG_UPD_REGION|DG_UPD_DRAWN;
		}
		else {
			gd<IfPolyDef>(n).whether = true;
			gd<PolyDef>(n) = readPolyDef(trans,att);
			ret.flags |= DG_UPD_POLYDEF;
		}
	}
	return ret.flags;
}
Update stringsIn(Transform *trans,Layout::Edge *e,const StrAttrs &attrs,bool clearOld) {
	StrAttrs allChanges;
	if(clearOld)
		clearRemoved(gd<StrAttrs>(e),attrs,allChanges);
	const StrAttrs &A = clearOld?allChanges:attrs;
	Update ret;
	EdgeGeom &eg = gd<EdgeGeom>(e);
	for(StrAttrs::const_iterator ai = A.begin(); ai!=A.end(); ++ai) {
		bool skip = false;
		if(ai->first=="pos") {
		    const DString &s = ai->second;
            if(s.empty()) {
                eg.pos.Clear();
            }
            else {
			    assert(s.length());
			    DString::size_type begin = 0,end=s.size();
                // grr i know i'll get this wrong in some obscure way...
                // just sloppily trying to throw away the [e,x,y ][s,x,y ]
                for(int j = 0; j<2; ++j) {
					if(s[begin]=='e'||s[begin]=='s') {
						++begin;
                        for(int i = 0; i<2; ++i) {
                            assert(s[begin]==',');
							++begin;
							// much too expansive: -6.2.-9.4 would be one number...
                            while(s[begin]=='-'||isdigit(s[begin])||s[begin]=='.')
								++begin;
                        }
					}
                    while(s[begin]==' ') ++begin;
                }
			    DString::size_type i = s.find(';',begin);
			    if(i!=DString::npos)
			        end = i;
			    Line newline;
			    istringstream stream(s.substr(begin,end-begin));
			    stream >> newline;
			    if(transformShape(trans,newline)) {
				    eg.pos = newline;
				    ret.flags |= DG_UPD_MOVE;
			    }
			    else
				    skip = true;
            }
		}
		if(!skip)
			gd<StrAttrs2>(e).put(ai->first,ai->second);
	}
	return ret;
}
void applyStrGraph(Transform *trans,bool useDotDefaults,StrGraph *g,Layout *out, Layout *subg) {
  stringsIn(trans,useDotDefaults,out,gd<StrAttrs>(g),false);
  gd<Name>(out) = gd<Name>(g);
  map<DString,Layout::Node*> renode;
  for(StrGraph::node_iter ni = g->nodes().begin(); ni!=g->nodes().end(); ++ni) {
    Layout::Node *n = out->create_node();
    subg->insert(n);
    renode[gd<Name>(n) = gd<Name>(*ni)] = n;
    StrAttrs &attrs = gd<StrAttrs>(*ni);
	stringsIn(trans,n,attrs,false);
  }
  for(StrGraph::graphedge_iter ei = g->edges().begin(); ei!=g->edges().end(); ++ei) {
    Layout::Edge *e = out->create_edge(renode[gd<Name>((*ei)->tail)],
				     renode[gd<Name>((*ei)->head)]).first;
    gd<Name>(e) = gd<Name>(*ei);
    subg->insert(e);
    StrAttrs &attrs = gd<StrAttrs>(*ei);
	stringsIn(trans,e,attrs,false);
  }
}
