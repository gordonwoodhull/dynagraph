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

#ifndef OutputIncrface_h
#define OutputIncrface_h

#include "incrout.h"

namespace Dynagraph {

template<typename Graph>
struct OutputIncrface : LinkedChangeProcessor<Graph> {
	dgr::reportType reportType_;
	OutputIncrface(ChangingGraph<Graph> *world,dgr::reportType report_type) : LinkedChangeProcessor<Graph>(world),reportType_(report_type) {}
	// ChangeProcessor
	void Process() {
		if(reports.enabled(reportType_)) {
			LOCK_REPORT(reportType_);
			emitChanges(reports[reportType_],this->world_->Q_);
		}
		this->NextProcess();
	}
};

} // namespace Dynagraph

#endif //OutputIncrface_h