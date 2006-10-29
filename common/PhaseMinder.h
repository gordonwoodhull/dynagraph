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

namespace Dynagraph {

// something to compare pulse times to see if a limit passed
struct PhaseMinder {
	typedef std::vector<DString> PhaseList;
	PhaseList phases_;
	PhaseMinder(const char *phases[],int nphases) {
		phases_.reserve(nphases);
		for(int i=0; i<nphases; ++i)
			phases_.push_back(phases[i]);
	}
#ifdef DEBUG_PHASEMINDER
	bool HasPassed2
#else
	bool HasPassed
#endif 
			(const StrAttrs &pulseAttrs,DString currPhase,DString currStep) {
		DString pulsePhase = pulseAttrs.look("phase");
		// if pulse/interrupt does not specify a phase, time limit has always passed
		if(!pulsePhase)
			return true;
		PhaseList::iterator pi = find(phases_.begin(),phases_.end(),pulsePhase),
			ci = find(phases_.begin(),phases_.end(),currPhase);
		// if either pulse or current phase is not in list, then limit has passed 
		if(pi==phases_.end() || ci==phases_.end())
			return true;
		// if pulse is earlier in list than current, then limit has passed
		if(pi<ci)
			return true;
		// however, if current is earlier, then limit has NOT passed
		if(pi>ci)
			return false;
		// ...phase is same...
		DString pulseStep = pulseAttrs.look("step");
		// if pulse does not specify step, then limit has passed
		if(!pulseStep)
			return true;
		// if step is same then limit has passed
		if(pulseStep==currStep)
			return true;
		// step is either "done" or an integer
		if(currStep=="done")
			return true;
		if(pulseStep=="done")
			return false;
		return atoi(pulseStep.c_str()) >= atoi(currStep.c_str());
	}
#ifdef DEBUG_PHASEMINDER
	bool HasPassed(const StrAttrs &pulseAttrs,DString currPhase,DString currStep) {
		DString pulsePhase = pulseAttrs.look("phase");
		reports[dgr::bug] << "pulse phase " << (pulsePhase?pulsePhase:"(none)") << " curr phase " << currPhase;
		bool res = HasPassed2(pulseAttrs,currPhase,currStep);
		reports[dgr::bug] << " result " << (res?"true":"false") << endl;
		return res;
	}
#endif
};

} // namespace Dynagraph
