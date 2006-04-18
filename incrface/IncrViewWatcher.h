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

#ifndef IncrViewWatcher_h
#define IncrViewWatcher_h

#include "common/ChangeQueue.h"

namespace Dynagraph {

template<typename Graph>
struct IncrViewWatcher {
	virtual ~IncrViewWatcher() {}
	virtual void IncrOpen(ChangeQueue<Graph> &Q) = 0;
	virtual void IncrClose(ChangeQueue<Graph> &Q) = 0;
	// do these belong here?  all i know is the implementation does not belong e.g. in IncrStrGraphHandler
	// and it neatly encapsulates all output in main's TextViewWatcher
	virtual void FulfilGraph(Graph *g) = 0;
	virtual void FulfilNode(typename Graph::Node *n) = 0;
	virtual void FulfilEdge(typename Graph::Edge *e) = 0;
};

} // namespace Dynagraph

#endif // IncrViewWatcher_h
