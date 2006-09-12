/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/


#ifndef TextWatcherOutput_h
#define TextWatcherOutput_h

#include "IncrStrGraphHandler.h"
#include "common/emitGraph.h"

namespace Dynagraph {

template<typename Graph>
struct TextWatcherOutput : IncrViewWatcher<Graph> {
	// IncrViewWatcher
	void FulfilGraph(Graph *g) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "fulfil graph " << gd<Name>(g) << std::endl;
		emitGraph(reports[dgr::output],g);
	}
	void FulfilNode(typename Graph::Node *n) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "fulfil node " << gd<Name>(n->g) << " " << gd<Name>(n) << " " << gd<StrAttrs>(n) << std::endl;
	}
	void FulfilEdge(typename Graph::Edge *e) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "fulfil edge " << gd<Name>(e->g) << " " << gd<Name>(e) << " " << gd<StrAttrs>(e) << std::endl;
	}
};

} // namespace Dynagraph

#endif //TextWatcherOutput_h
