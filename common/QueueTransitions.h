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

#include "Constraints.h"

namespace Dynagraph {

template<typename Graph1,typename Graph2>
struct GoingQueueTransition {
	ChangeQueue<Graph2> Q_;
	GoingQueueTransition(Graph2 *world2,Graph2 *current2) : Q_(world2,current2) {}
	GoingQueueTransition(const GoingQueueTransition &other) : Q_(other.Q_.whole,other.Q_.current) {
		Q_ = other.Q_;
	}
	ChangeQueue<Graph2> &NextQ() {
		return Q_;
	}
	bool CheckRedundancy() {
		return true;
	}
	void EndLastQ(ChangeQueue<Graph1> &Q) {}
};
template<typename Graph1,typename Graph2>
class ReturningQueueTransition {
	ChangeQueue<Graph2> *nextQ_;
public:
	void AssignNext(ChangeQueue<Graph2> *nextQ) {
		nextQ_ = nextQ;
	}
	ChangeQueue<Graph2> &NextQ() {
		return *nextQ_;
	}
	bool CheckRedundancy() {
		return false;
	}
	void EndLastQ(ChangeQueue<Graph1> &Q) {
		Q.Execute(true);
	}
};

} // namespace Dynagraph

#endif // QueueTransitions_h
