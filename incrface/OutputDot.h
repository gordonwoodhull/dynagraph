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

#ifndef OutputDot_h
#define OutputDot_h

#include "incrout.h"

namespace Dynagraph {

template<typename Graph>
struct OutputDot : LinkedChangeProcessor<Graph> {
	OutputDot(ChangingGraph<Graph> *world) : LinkedChangeProcessor<Graph>(world) {}
	// ChangeProcessor
	void Open() {
		if(reports.enabled(dgr::dotout)) {
			LOCK_REPORT(dgr::dotout);
            emitGraph(reports[dgr::dotout],&this->world_->current_);
		}
		this->NextOpen();
	}
	void Process() {
		if(reports.enabled(dgr::dotout)) {
			LOCK_REPORT(dgr::dotout);
            emitGraph(reports[dgr::dotout],&this->world_->current_);
		}
		this->NextProcess();
	}
	// no way to express Close or Pulse
};

} // namespace Dynagraph

#endif //OutputDot_h
