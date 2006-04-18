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

#ifndef dstringxep_h
#define dstringxep_h

#include "dgxep.h"

namespace Dynagraph {

struct DictStringLost : DGException2 {
	// I don't *think* this could cause some horrible recursive crash..!?
	DictStringLost(DString s) : DGException2("StringDict internal exception: string lost",s,true) {}
};

} // namespace Dynagraph

#endif // dstringxep_h
