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


#include "info.h"

namespace Voronoi {


Site *Sites::getsite() {
    return fsites.alloc();
}

void Sites::makevertex(Site *v) {
    v -> sitenbr = nvertices++;
}


void Sites::deref(Site *v) {
    v -> refcnt -= 1;
    if (v -> refcnt == 0 )
		fsites.free(v);
}

void Sites::ref(Site *v) {
    v -> refcnt += 1;
}

} // namespace Voronoi
