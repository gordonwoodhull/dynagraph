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
	void Open() {
		if(reports.enabled(reportType_)) {
			LOCK_REPORT(reportType_);
			reports[reportType_] << "open graph " << gd<Name>(&this->world_->whole_) << " " << gd<StrAttrs>(&this->world_->whole_) << std::endl;
		}
		this->NextOpen();
	}
	void Process() {
		if(reports.enabled(reportType_)) {
			LOCK_REPORT(reportType_);
			emitChanges(reports[reportType_],this->world_->Q_);
		}
		this->NextProcess();
	}
	void Close() {
		if(reports.enabled(reportType_)) {
			LOCK_REPORT(reportType_);
			reports[reportType_] << "close graph " << gd<Name>(&this->world_->whole_) << std::endl;
		}
	}
	void Pulse(const StrAttrs &attrs) {
		if(reports.enabled(reportType_)) {
			LOCK_REPORT(reportType_);
			reports[reportType_] << "pulse graph " << gd<Name>(&this->world_->whole_) << " " << attrs << std::endl;
		}
		if(reports.enabled(dgr::input_cooked)) {
			LOCK_REPORT(dgr::input_cooked);
			reports[dgr::input_cooked] << "pulse graph " << gd<Name>(&this->world_->current_) << ' ' << attrs << std::endl;
		}
	}
};

} // namespace Dynagraph

#endif //OutputIncrface_h
