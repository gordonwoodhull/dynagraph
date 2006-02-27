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


#ifndef HEDGES_H
#define HEDGES_H

#include "site.h"
#include "edges.h"

namespace Dynagraph {
namespace Voronoi {

struct Halfedge {
    Halfedge    *ELleft, *ELright;
    Edge               *ELedge;
    int                ELrefcnt;
    EdgeEnd            ELpm;
    Site               *vertex;
    double              ystar;
    Halfedge    *PQnext;
};

struct Halfedges {
	Halfedge *leftend, *rightend;
	Freelist<Halfedge> fhedges;
	std::vector<Halfedge*> hash;
	Rect range; // of site coords
	Site *bottomsite; // weird to hold this here but this is the only class that uses it
	Sites &sites;

	Halfedges(Sites &sites,int N);
	void init(int N);
	Site *hintersect(Halfedge*, Halfedge*);
	Halfedge *create(Edge*, EdgeEnd);
	void insert(Halfedge *where, Halfedge *he);
	Halfedge *leftbnd(Coord);
	void erase(Halfedge *);
	Halfedge *ELleftbnd(Coord);
	Site *leftreg(Halfedge*), *rightreg(Halfedge*);

	void PQinitialize();
	Halfedge * PQextractmin();
	Coord PQ_min();
	int PQempty();
	void PQdelete(Halfedge*);
	void PQinsert(Halfedge*, Site*, double);

private:
	Halfedge *gethash(int b);

	std::vector<Halfedge> PQhash;
	int PQhashsize;
	int PQcount;
	int PQmin;

	int PQbucket(Halfedge *he);
};
bool right_of(Halfedge*, Coord);
inline Halfedge *left(Halfedge*he) {
	return he->ELleft;
}
inline Halfedge *right(Halfedge*he) {
	return he->ELright;
}

} // namespace Voronoi
} // namespace Dynagraph

#endif
