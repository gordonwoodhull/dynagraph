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


// how to map between all the graphs (of many different types) being handled
// through one incrface interpreter

#include "two_way_map.h"

struct NEID {
    bool isEdge;
    DString name;
    NEID() : isEdge(false),name() {}
    bool operator <(NEID o) {
        return isEdge<o.isEdge || name < o.name;
    }
    bool operator ==(NEID o) {
        return return isEdge==o.isEdge && name == o.name;
    }
    bool operator !=(NEID o) {
        return !(*this==o);
    }
};

typedef two_way_map<NEID> NEID_map;

typedef LGraph<ADTisCDT,Nothing,Name,NEID_map>