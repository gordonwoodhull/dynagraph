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

#ifndef DynaDAGLayout_h
#define DynaDAGLayout_h

#include "common/LayoutAttrs.h"

namespace Dynagraph {
namespace DynaDAG {

// until DynaDAGEngine becomes a template, this is the minimal graph type 
// that can be used both for incrface and by the engine
typedef NamedGraph<ADTisCDT,EverythingGraphAttrs,EverythingNodeAttrs,EverythingEdgeAttrs,
	LayoutUpdates,LayoutUpdates,LayoutUpdates> DynaDAGLayout;

} // namespace DynaDAG
} // namespace Dynagraph


#endif // DynaDAGLayout_h
