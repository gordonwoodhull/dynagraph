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


#ifndef INFO_H
#define INFO_H

#include "common/Dynagraph.h"
#include "common/freelist.h"
#include "site.h"

namespace Voronoi {

struct PtItem {           /* Point std::list */
	PtItem*    next;
	Coord             p;
	PtItem() : next(0) {}
};
struct Info {                  /* Info concerning site */
	Layout::Node *layoutN;     /* libgraph node */
	Site site;                 /* site used by voronoi code */
	bool overlaps;             /* true if node overlaps other nodes */
	PtItem *verts;             /* sorted std::list of vertices of */
							   /* voronoi polygon */
	Info() : layoutN(0),overlaps(false),verts(0) {}
};
struct Infos {
	Freelist<PtItem> fpoints;


	std::vector<Info> nodes;			/* Array of node info */

	Infos(int N) : fpoints(ROUND(sqrt((double)N))),nodes(N) {}

	/* Insert vertex into sorted std::list */
	void addVertex (Site*, Coord);
};
inline double dist(Site *s, Site *t) {
	return ::dist(s->coord,t->coord);
}

} // namespace Voronoi
#endif // INFO_H
