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


#ifndef SITE_H
#define SITE_H

#include "common/Geometry.h"
#include "common/freelist.h"

namespace Voronoi {

struct Site {
	Coord coord;
	int sitenbr;
	int refcnt;
	Site() : sitenbr(-1),refcnt(0) {}
};

struct Sites {
	Freelist<Site> fsites;
	int nvertices;
	Site *getsite();
	void makevertex(Site*);     /* Transform a site into a vertex */
	void deref(Site*);
	void ref(Site*);

	Sites(int N) : fsites(ROUND(sqrt((double)N))), nvertices(0) {}

};

} // namespace Voronoi
#endif // SITE_H
