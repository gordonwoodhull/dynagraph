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


#include "voronoi.h"

using namespace std;

namespace Dynagraph {
namespace Voronoi {

void VoronoiServer::voronoi(const vector<Site*> &order) {
    vector<Site*>::const_iterator is = order.begin();
    Site *newsite, *bot, *top, *p;
    Site *v;
    Coord newintstar;
    EdgeEnd pm;
    Halfedge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
    Edge *e;

    if(is==order.end())
        return;

#ifdef VORLINES
    gd<Drawn>(this->world_->current_).clear();
#endif
    edges.fedges.clear();
    sites.fsites.clear();
    hedges.fhedges.clear();
    hedges.bottomsite = *is++;
    hedges.init(this->world_->current_.nodes().size());

    newsite = is==order.end()?0:*is++;
    while(1) {
        if(!hedges.PQempty())
            newintstar = hedges.PQ_min();

        if (newsite != (struct Site *)0
           && (hedges.PQempty()
             || newsite -> coord.y < newintstar.y
             || (newsite->coord.y == newintstar.y
                 && newsite->coord.x < newintstar.x))) {
    /* new site is smallest */
            lbnd = hedges.leftbnd(newsite->coord);
            rbnd = right(lbnd);
            bot = hedges.rightreg(lbnd);
            e = edges.bisect(bot, newsite);
            bisector = hedges.create(e, le);
            hedges.insert(lbnd, bisector);
            if((p = hedges.hintersect(lbnd, bisector))) {
                hedges.PQdelete(lbnd);
                hedges.PQinsert(lbnd, p, dist(p,newsite));
            }
            lbnd = bisector;
            bisector = hedges.create(e, re);
            hedges.insert(lbnd, bisector);
            if((p = hedges.hintersect(bisector, rbnd)))
                hedges.PQinsert(bisector, p, dist(p,newsite));
            newsite = is==order.end()?0:*is++;
        }
        else if(!hedges.PQempty()) {
    /* intersection is smallest */
            lbnd = hedges.PQextractmin();
            llbnd = left(lbnd);
            rbnd = right(lbnd);
            rrbnd = right(rbnd);
            bot = hedges.leftreg(lbnd);
            top = hedges.rightreg(rbnd);
#ifdef STANDALONE
            out_triple(bot, top, rightreg(lbnd));
#endif
            v = lbnd->vertex;
            sites.makevertex(v);
            edges.endpoint(lbnd->ELedge,lbnd->ELpm,v);
            edges.endpoint(rbnd->ELedge,rbnd->ELpm,v);
            hedges.erase(lbnd);
            hedges.PQdelete(rbnd);
            hedges.erase(rbnd);
            pm = le;
            if (bot->coord.y > top->coord.y) {
                swap(bot,top);
                pm = re;
            }
            e = edges.bisect(bot, top);
            bisector = hedges.create(e, pm);
            hedges.insert(llbnd, bisector);
            edges.endpoint(e, opp(pm), v);
            sites.deref(v);
            if((p = hedges.hintersect(llbnd, bisector))) {
                hedges.PQdelete(llbnd);
                hedges.PQinsert(llbnd, p, dist(p,bot));
            }
            if((p = hedges.hintersect(bisector, rrbnd))) {
                hedges.PQinsert(bisector, p, dist(p,bot));
            }
        }
        else break;
    }

    for(lbnd=right(hedges.leftend); lbnd != hedges.rightend; lbnd=right(lbnd)) {
        e = lbnd -> ELedge;
        edges.clip_line(e);
#ifdef STANDALONE
        out_ep(e);
#endif
    }
}

} // namespace Voronoi
} // namespace Dynagraph
