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

#ifndef WorldInABox_h
#define WorldInABox_h

#include "common/InternalTranslator.h"
#include "common/StringLayoutTranslator.h"
#include "common/SimpleEngines.h"
#include "common/EnginePair.h"
#include "OutputIncrface.h"

namespace Dynagraph {

// Simple helper for when a chain is not really a chain
template<typename Graph>
struct SkipEngine : LinkedChangeProcessor<Graph> {
	SkipEngine(ChangingGraph<Graph> *world,LinkedChangeProcessor<Graph> *next=0) 
		: LinkedChangeProcessor<Graph>(world,next) {}
	~SkipEngine() {
		next_ = 0; // do not chain deletion (?!)
	}
	void Process() {
		next_->next_->Process();
	}
};

template<typename OuterLayout,typename InnerLayout,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<OuterLayout> {
	ChangingGraph<InnerLayout> *innerWorld_;
	LinkedChangeProcessor<OuterLayout> *inXlator_;

	WorldInABox(ChangingGraph<OuterLayout> *outerWorld,ChangingGraph<InnerLayout> *innerWorld) 
		: LinkedChangeProcessor<OuterLayout>(outerWorld),innerWorld_(innerWorld),inXlator_(0) {
	}
	~WorldInABox() {
		// continue chain of deletion in
		if(inXlator_)
			delete inXlator_;
		// deletion continues through outTranslator but stops at SkipEngine
		delete innerWorld_;
		// then continues in my ~LinkedChangeProcessor to my next_
	}
	// all of this seems like it should be Configurator work
	void assignEngine(EnginePair<InnerLayout> innerEngines,Transform *transform) {
		innerEngines.Prepend(new UpdateCurrentProcessor<InnerLayout>(innerWorld_));
		
		// debug loggers to see what the "inner engine" is doing
		// (which require engines to "stringize" attributes before them)
		typedef InternalTranslator2<InnerLayout,LayoutToStringTranslator<InnerLayout,InnerLayout> > MakeStrings;
		if(reports.enabled(dgr::inner_input)) {
			OutputIncrface<InnerLayout> *logIn = new OutputIncrface<InnerLayout>(innerWorld_,dgr::inner_input);
			innerEngines.Prepend(logIn);
			MakeStrings *makeStrings = new MakeStrings(innerWorld_,transform);
			innerEngines.Prepend(makeStrings);
		}
		if(reports.enabled(dgr::inner_output)) {
			MakeStrings *makeStrings = new MakeStrings(innerWorld_,transform);
			innerEngines.Append(makeStrings);
			OutputIncrface<InnerLayout> *logOut = new OutputIncrface<InnerLayout>(innerWorld_,dgr::inner_output);
			innerEngines.Append(logOut);
		}
		// translators to/from outside world
		InTranslator *inTranslator = new InTranslator(this->world_,innerWorld_);
		OutTranslator *outTranslator = new OutTranslator(innerWorld_,this->world_);
		innerEngines.Prepend(inTranslator);
		innerEngines.Append(outTranslator);
		// something to clear outside world's changes
		LinkedChangeProcessor<OuterLayout> *okayEngine  = new OkayEngine<OuterLayout>(this->world_);
		// and something to tie back to my next_
		LinkedChangeProcessor<OuterLayout> *delegator = new SkipEngine<OuterLayout>(this->world_,this);
		static_cast<LinkedChangeProcessor<OuterLayout>*>(outTranslator)->next_ = okayEngine;
		okayEngine->next_ = delegator;
		inXlator_ = inTranslator;
	}
	void Process() {
		inXlator_->Process();
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
