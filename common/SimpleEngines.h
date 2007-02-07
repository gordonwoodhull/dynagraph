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

#ifndef SimpleEngines_h 
#define SimpleEngines_h 

#include "ChangeProcessor.h"

namespace Dynagraph {

// changes don't automatically get cleared, so that engines can hold changes open for intermediate layout
// so every world needs an engine that does clear changes.  dynadag does.  or this does:
template<typename Graph>
struct OkayEngine : ChangeProcessor<Graph> {
	OkayEngine(ChangingGraph<Graph> *world) : ChangeProcessor<Graph>(world) {}
	void Open(ChangeProcessing *next) {
		this->NextOpen();
		this->world_->Q_.Clear();
	}
	void Process(ChangeProcessing *next) {
		this->NextProcess();
		this->world_->Q_.ExecuteDeletions();
		this->world_->Q_.Clear();
	}
};

// simple server that just updates the current subgraph based on changes.
// this must be done only once, that's why individual layout servers can't be responsible.
template<typename Graph>
struct UpdateCurrentProcessor : ChangeProcessor<Graph> {
	UpdateCurrentProcessor(ChangingGraph<Graph> *world) 
		: ChangeProcessor<Graph>(world) {}
	void Process(ChangeProcessing *next) {
		this->world_->Q_.UpdateCurrent();
		this->NextProcess();
	}
};

} // namespace Dynagraph

#endif // SimpleEngines_h 
