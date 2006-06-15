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


#ifndef QueueTransitions_h
#define QueueTransitions_h

namespace Dynagraph {

template<typename Graph1>
struct GoingQueueTransition {
	static bool CheckRedundancy() {
		return true;
	}
	static void EndLastQ(ChangeQueue<Graph1> &Q) {}
};
template<typename Graph1>
struct ReturningQueueTransition {
	static bool CheckRedundancy() {
		return false;
	}
	static void EndLastQ(ChangeQueue<Graph1> &Q) {
	}
};

} // namespace Dynagraph

#endif // QueueTransitions_h
