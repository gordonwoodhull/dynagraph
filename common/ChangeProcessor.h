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

#ifndef ChangeProcessor_h
#define ChangeProcessor_h

#include "Modify.h"
#include "ChangingGraph.h"

namespace Dynagraph {

// a ChangeProcessor client should send changes on without expecting any response
// the server is allowed to change the Q and is expected to eventually realize the changes
template<typename Graph>
struct ChangeProcessor {
    ChangingGraph<Graph> * const world_;
	typedef Graph GraphType;
	ChangeProcessor(ChangingGraph<Graph> *world) : world_(world) {}
	virtual void Process() = 0;
	virtual ~ChangeProcessor() {}
};
template<typename Graph>
struct LinkedChangeProcessor : ChangeProcessor<Graph> {
	LinkedChangeProcessor<Graph> *next_;
	LinkedChangeProcessor(ChangingGraph<Graph> *world,LinkedChangeProcessor<Graph> *next=0)
		: ChangeProcessor<Graph>(world),next_(next) {}
	virtual ~LinkedChangeProcessor() {
		if(next_)
			delete next_;
	}
	void NextProcess() {
		if(next_)
			next_->Process();
	}
};
// a ChangeTranslator is the end of one chain of processors and the beginning of another
template<typename Graph1,typename Graph2>
struct ChangeTranslator : LinkedChangeProcessor<Graph1>, LinkedChangeProcessor<Graph2> {
	ChangeTranslator(ChangingGraph<Graph1> *world1,ChangingGraph<Graph2> *world2)
		: LinkedChangeProcessor<Graph1>(world1),LinkedChangeProcessor<Graph2>(world2) {}
	// LinkedChangeProcessor<Graph1>::next_ must be null
};

template<typename Graph>
struct EnginePair : std::pair<LinkedChangeProcessor<Graph>*,LinkedChangeProcessor<Graph>*> {
	EnginePair() {}
	EnginePair(LinkedChangeProcessor<Graph> *first,LinkedChangeProcessor<Graph> *second) {
		this->first = first;
		this->second = second;
	}
	void Prepend(LinkedChangeProcessor<Graph> *eng) {
		eng->next_ = this->first;
		this->first = eng;
	}
	void Prepend(const EnginePair<Graph> &other) {
		other.second->next_ = this->first;
		this->first = other.first;
	}
	void Append(LinkedChangeProcessor<Graph> *eng) {
		this->second->next_ = eng;
		this->second = eng;
	}
	void Append(const EnginePair<Graph> &other) {
		this->second->next_ = other.first;
		this->second = other.second;
	}
};


// simple server that just updates the current subgraph based on changes.
// this must be done only once, that's why individual layout servers can't be responsible.
template<typename Graph>
struct UpdateCurrentProcessor : LinkedChangeProcessor<Graph> {
	UpdateCurrentProcessor(ChangingGraph<Graph> *world) 
		: LinkedChangeProcessor<Graph>(world) {}
	void Process() {
		this->world_->Q_.UpdateCurrent();
		this->NextProcess();
	}
};

} // namespace Dynagraph

#endif //ChangeProcessor_h
