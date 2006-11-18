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


#ifndef EDGES_H
#define EDGES_H

#include "site.h"
#include "info.h"

namespace Dynagraph {
namespace Voronoi {

struct Edge {
    double      a,b,c;         /* edge on line ax + by = c */
    Site       *ep[2];        /* endpoints (vertices) of edge; initially NULL */
    Site       *reg[2];       /* sites forming edge */
    int        edgenbr;
};

typedef enum EdgeEnds_ {le = 0, re = 1} EdgeEnd;
inline EdgeEnd opp(EdgeEnd e) {
	if(e==le)
		return re;
	else
		return le;
}

struct Edges {
	Freelist<Edge> fedges;
	int nedges;
	Sites &sites;
	Infos &infos;
	Bounds &bounds;

	Edges(Sites &sites,Infos &infos,Bounds &bounds,int N) :
	  fedges(ROUND(sqrt((double)N))),
	  nedges(0),
	  sites(sites),
	  infos(infos),
	  bounds(bounds) {}

	void endpoint(Edge*, EdgeEnd, Site*);
	void clip_line(Edge *e);
	Edge *bisect(Site*, Site*);
private:
	void doSeg (Edge *e, double x1, double y1, double x2, double y2);
};

struct Whattux {};

} // namespace Voronoi
} // namespace Dynagraph

#endif
