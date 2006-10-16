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

// ChangeProcessor, a.k.a. Engine
// a ChangeProcessor client should send changes on without expecting any response
// the server is allowed to change the Q and is expected to eventually realize the changes
template<typename Graph>
struct ChangeProcessor {
    ChangingGraph<Graph> * const world_;
	typedef Graph GraphType;
	ChangeProcessor(ChangingGraph<Graph> *world) : world_(world) {}
	virtual void Open() = 0;
	virtual void Process() = 0;
	virtual void Close() = 0;
	virtual void Pulse(const StrAttrs &attrs) = 0;
	virtual ~ChangeProcessor() {}
	typedef void (ChangeProcessor::*Function)();
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
	void NextOpen() {
		if(next_)
			next_->Open();
	}
	void NextProcess() {
		if(next_)
			next_->Process();
	}
	void NextClose() {
		if(next_)
			next_->Close();
	}
	void NextPulse(const StrAttrs &attrs) {
		if(next_)
			next_->Pulse(attrs);
	}
	// default implementations (almost no one cares about Open or Close)
	void Open() {
		NextOpen();
	}
	void Process() {
		NextProcess();
	}
	void Close() {
		NextClose();
	}
	void Pulse(const StrAttrs &attrs) {
		NextPulse(attrs);
	}
};

} // namespace Dynagraph

#endif //ChangeProcessor_h
