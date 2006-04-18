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
#include "stringsIn.h"
#include "genpoly.h"
#include <sstream>

using namespace std;

namespace Dynagraph {

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
// when the clearOld flag is set, every attribute that existed before but is
// not in the apply set should be set = "".  this adds those commands.
void clearRemoved(const StrAttrs &current,const StrAttrs &apply,StrAttrs &ret) {
    ret = apply;
	for(StrAttrs::const_iterator ai = current.begin(); ai!=current.end(); ++ai)
        ret[ai->first]; // referencing will create w/ value blank if wasn't in apply
}
void ensureAttr(const StrAttrs &att,StrAttrs &A, DString name) {
    // make sure that some attributes don't get left unset
    // by setting them to "" if they're not there
    if(att.find(name)==att.end())
        A[name]; // lookup creates blank entry if not there
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

} // namespace Dynagraph
