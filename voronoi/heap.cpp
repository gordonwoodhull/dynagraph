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


#include <stdio.h>
#include "hedges.h"

namespace Voronoi {

int Halfedges::PQbucket(Halfedge *he) {
    int bucket;

    bucket = int((he->ystar - range.b)/range.Height() * PQhashsize);
    if (bucket<0) bucket = 0;
    if (bucket>=PQhashsize) bucket = PQhashsize-1 ;
    if (bucket < PQmin) PQmin = bucket;
    return(bucket);
}

void Halfedges::PQinsert(Halfedge *he, Site *v, double offset) {
    Halfedge  *last, *next;

    he -> vertex = v;
    sites.ref(v);
    he -> ystar = v -> coord.y + offset;
    last = &PQhash[PQbucket(he)];
    while ((next = last -> PQnext) != (struct Halfedge *) NULL &&
        (he -> ystar  > next -> ystar  ||
        (he -> ystar == next -> ystar && v -> coord.x > next->vertex->coord.x)))
      {
        last = next;
      }
    he -> PQnext = last -> PQnext;
    last -> PQnext = he;
    PQcount += 1;
}

void Halfedges::PQdelete(Halfedge *he) {
    Halfedge *last;

    if(he ->  vertex != (Site *) NULL) {
        last = &PQhash[PQbucket(he)];
        while (last -> PQnext != he) last = last -> PQnext;
        last -> PQnext = he -> PQnext;
        PQcount -= 1;
        sites.deref(he -> vertex);
        he -> vertex = (Site *) NULL;
    }
}


int Halfedges::PQempty() {
    return(PQcount==0);
}


Coord Halfedges::PQ_min() {
    Coord answer;

    while(PQhash[PQmin].PQnext == (struct Halfedge *)NULL) {PQmin += 1;}
    answer.x = PQhash[PQmin].PQnext -> vertex -> coord.x;
    answer.y = PQhash[PQmin].PQnext -> ystar;
    return answer;
}

Halfedge *Halfedges::PQextractmin() {
    Halfedge *curr;

    curr = PQhash[PQmin].PQnext;
    PQhash[PQmin].PQnext = curr -> PQnext;
    PQcount -= 1;
    return(curr);
}


void Halfedges::PQinitialize() {
    int i;

    PQcount = 0;
    PQmin = 0;
    PQhash.resize(PQhashsize);
    for(i=0; i<PQhashsize; i+=1)
		PQhash[i].PQnext = 0;
}

/*
static void
PQdumphe (Halfedge *p)
{
    printf ("  [%x] %x %x %d %d %d %d %f\n",
      p, p->ELleft, p->ELright, p->ELedge->edgenbr,
      p->ELrefcnt, p->ELpm, (p->vertex ? p->vertex->sitenbr : -1), p->ystar);
}

void PQdump()
{
    int i;
    Halfedge *p;

    for(i=0; i<PQhashsize; i+=1) {
      printf("[%d]\n", i);
      p = PQhash[i].PQnext;
      while (p != NULL) {
        PQdumphe (p);
        p = p->PQnext;
      }
    }

}
*/

} // namespace Voronoi

