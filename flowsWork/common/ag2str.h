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

#ifndef ag2str_h
#define ag2str_h

// a silly translation deal until i make a native agread

#include "LGraph-cdt.h"
#include "StrAttr.h"

extern "C" {
	#include "agraph.h"
};

namespace Dynagraph {

StrGraph *ag2str(Agraph_t *g);
Agraph_t *str2ag(StrGraph *gg);

// struct agreadError {};
inline StrGraph *readStrGraph(FILE *f) {
	Agraph_t *g = agread(f,0);
	if(!g)
		return 0;//throw agreadError();
	return ag2str(g);
}

} // namespace Dynagraph

#endif //ag2str_h
