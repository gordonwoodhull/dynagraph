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
#include "OutputIncrface.h"

namespace Dynagraph {

// not entirely pretty: yes it makes sense to have an engine that just clears Qs
// (if it's the responsibility of engines to clear Qs, which seems to be so)
// but what's up with whoseNext?  it's just there so that WorldInABox can look like a regular engine
template<typename Layout>
struct OkayEngine : LinkedChangeProcessor<Layout> {
	LinkedChangeProcessor<Layout> *whoseNext_;
	OkayEngine(ChangingGraph<Layout> *world,LinkedChangeProcessor<Layout> *whoseNext) : LinkedChangeProcessor<Layout>(world),whoseNext_(whoseNext) {}
	void Process() {
		whoseNext_->next_->Process();
		this->world_->Q_.ExecuteDeletions();
		this->world_->Q_.Clear();
	}
};
template<typename OuterLayout,typename InnerLayout,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<OuterLayout> {
	ChangingGraph<InnerLayout> inWorld_;
	LinkedChangeProcessor<OuterLayout>  *inXlator_,*outXlator_,*okayEngine_;

	WorldInABox(ChangingGraph<OuterLayout> *world) 
		: LinkedChangeProcessor<OuterLayout>(world) {
		gd<Name>(&inWorld_.whole_) = "INR";
	}
	~WorldInABox() {
		// chain of deletion will get outXlator and okayEngine automagically
		if(inXlator_)
			delete inXlator_;
	}
	void assignEngine(DString engines,Transform *transform) {
		EnginePair<InnerLayout> innerEngines = createEngine(engines,&inWorld_);
		// debug loggers to see what the "inner engine" is doing
		// (which require engines to "stringize" attributes before them)
		typedef InternalTranslator2<InnerLayout,LayoutToStringTranslator<InnerLayout,InnerLayout> > MakeStrings;
		if(reports.enabled(dgr::inner_input)) {
			OutputIncrface<InnerLayout> *logIn = new OutputIncrface<InnerLayout>(&inWorld_,dgr::inner_input);
			innerEngines.Prepend(logIn);
			MakeStrings *makeStrings = new MakeStrings(&inWorld_,transform);
			innerEngines.Prepend(makeStrings);
		}
		if(reports.enabled(dgr::inner_output)) {
			MakeStrings *makeStrings = new MakeStrings(&inWorld_,transform);
			innerEngines.Append(makeStrings);
			OutputIncrface<InnerLayout> *logOut = new OutputIncrface<InnerLayout>(&inWorld_,dgr::inner_output);
			innerEngines.Append(logOut);
		}
		// translators to/from outside world
		InTranslator *inTranslator = new InTranslator(this->world_,&inWorld_);
		OutTranslator *outTranslator = new OutTranslator(&inWorld_,this->world_);
		innerEngines.Prepend(inTranslator);
		innerEngines.Append(outTranslator);
		// something to clear outside world's changes (weird!)
		okayEngine_  = new OkayEngine<OuterLayout>(this->world_,this);
		static_cast<LinkedChangeProcessor<OuterLayout>*>(outTranslator)->next_ = okayEngine_;
		inXlator_ = inTranslator;
		outXlator_ = outTranslator;
	}
	void Process() {
		inXlator_->Process();
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
