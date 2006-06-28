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

#ifndef ChangingGraph_h
#define ChangingGraph_h

namespace Dynagraph {

template<typename Graph>
struct ChangingGraph {
	Graph whole_,current_;
	ChangeQueue<Graph> Q_;
	ChangingGraph() : current_(&whole_),Q_(&whole_,&current_) {}
};

} // namespace Dynagraph

#endif // ChangingGraph_h
