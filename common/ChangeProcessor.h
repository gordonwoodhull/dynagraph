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

// Each method has a parameter next that points to an implementation in the layout manager that knows what to call next
// That implementation should ignore the next parameter
// It is imperative, and the default behavior, to call next 
// (In order to allow multiple call-forwards, manager can't do that.)

// the virtual methods, implemented by both manager and processor
struct ChangeProcessing {
	virtual ~ChangeProcessor() = 0; // look! a virtual destructor that actually gets used!
	virtual void Open(ChangeProcessing *next) = 0;
	virtual void Process(ChangeProcessing *next) = 0;
	virtual void Close(ChangeProcessing *next) = 0;
	virtual void Pulse(ChangeProcessing *next,const StrAttrs &attrs) = 0;
	typedef virtual void (ChangeProcessor::*Function)(ChangeProcessing *next);
};
template<typename Graph>
struct ChangeProcessor : ChangeProcessing {
    ChangingGraph<Graph> * const world_;
	typedef Graph GraphType;
	ChangeProcessor(ChangingGraph<Graph> *world) : world_(world) {}
	virtual void Open(ChangeProcessing *next) {
		next->Open(0);
	}
	virtual void Process(ChangeProcessing *next) {
		next->Process(0);
	}
	virtual void Close(ChangeProcessing *next) {
		next->Close(0);
	}
	virtual void Pulse(ChangeProcessing *next,const StrAttrs &attrs) {
		next->Pulse(0,attrs);
	}
};

} // namespace Dynagraph

#endif //ChangeProcessor_h
