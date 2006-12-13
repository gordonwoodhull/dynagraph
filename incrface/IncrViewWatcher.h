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

// this is a weird interface that is slowly dwindling into nothing
template<typename Graph>
struct IncrViewWatcher {
	virtual ~IncrViewWatcher() {}
	virtual void FulfilGraph(Graph *g) = 0;
	virtual void FulfilNode(typename Graph::Node *n) = 0;
	virtual void FulfilEdge(typename Graph::Edge *e) = 0;
};

} // namespace Dynagraph

#endif // IncrViewWatcher_h
