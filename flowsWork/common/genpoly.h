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


#ifndef genpoly_h
#define genpoly_h

#include "Geometry.h"

namespace Dynagraph {

/*
 *	shape generators
 */
struct PolyDef {
	bool isEllipse;
	double aspect; // for ellipses only (y/x)
	int sides, // for polys only
		peripheries;
	double perispacing,
		rotation,
		skew,distortion;
	bool regular;
	Coord interior_box, // minimum inside size (e.g. for text)
		exterior_box; // minimum outside size
	Line input; // overrides sides,input
	PolyDef() : isEllipse(false), aspect(1),sides(4),peripheries(0),perispacing(0),
		rotation(0),skew(0),distortion(0),regular(false),
		interior_box(0,0),exterior_box(0,0) {
	}
};

extern void genpoly(const PolyDef &arg,Lines &out);
Coord polysize(const Line &poly);

// exceptions
struct GenPolyXep : DGException {
	GenPolyXep(const char *descrip) : DGException(descrip) {}
};
struct BadPolyBounds : GenPolyXep {
  BadPolyBounds() : GenPolyXep("must specify internal or external box of poly; no one-dimensional or negative boxes") {}
};
struct BadPolyDef : GenPolyXep {
  BadPolyDef() : GenPolyXep("polygon must have at least three sides, or ellipse must have aspect") {}
};
struct BadInputPoly : GenPolyXep {
    BadInputPoly() : GenPolyXep("polydef input poly must have degree>0 and size>2") {}
};

//bezier_t *genellipse(polyreq_t *arg);

} // namespace Dynagraph

#endif // genpoly_h
