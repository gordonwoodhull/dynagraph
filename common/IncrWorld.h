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

#ifndef DynagraphWorld_h
#define DynagraphWorld_h

namespace Dynagraph {

template<typename Graph>
struct DynagraphWorld {
	Graph whole_,current_;
	DynagraphWorld() : current_(&whole_) {}
};

} // namespace Dynagraph

#endif // DynagraphWorld_h
