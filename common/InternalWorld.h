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

#ifndef InternalWorld_h
#define InternalWorld_h

#include <list>

#include "ChangeProcessor.h"
#include "ChangeTranslator.h"
#include "EnginePair.h"

namespace Dynagraph {

template<typename OuterLayout,typename InnerLayout>
struct InternalWorld : LinkedChangeProcessor<OuterLayout> {
	ChangingGraph<InnerLayout> *innerWorld_;
	EnginePair<InnerLayout> innerEngines_;
	typedef ChangeTranslator<OuterLayout,InnerLayout> InTranslator;
	typedef ChangeTranslator<InnerLayout,OuterLayout> OutTranslator;
	typedef std::list<InTranslator*> InTranslatorList;
	typedef std::list<OutTranslator*> OutTranslatorList;
	InTranslatorList inTranslators_;
	OutTranslatorList outTranslators_;

	struct InnerCatcher : LinkedChangeProcessor<InnerLayout> {
		InternalWorld *me_;
		InnerCatcher(InternalWorld *me) : LinkedChangeProcessor<InnerLayout>(me->innerWorld_),me_(me) {}
		void Open() {
			me_->innerOpenOut();
		}
		void Process() {
			me_->innerProcessOut();
		}
		void Close() {
			me_->innerCloseOut();
		}
		void Pulse(const StrAttrs &attrs) {
			me_->innerPulseOut(attrs);
		}

	};
	InternalWorld(ChangingGraph<OuterLayout> *outerWorld,ChangingGraph<InnerLayout> *innerWorld)
		: LinkedChangeProcessor<OuterLayout>(outerWorld),innerWorld_(innerWorld) {
	}
	~InternalWorld() {
		for(typename InTranslatorList::iterator ti = inTranslators_.begin(); ti!=inTranslators_.end(); ++ti)
			delete *ti;
		for(typename OutTranslatorList::iterator ti = outTranslators_.begin(); ti!=outTranslators_.end(); ++ti)
			delete *ti;
		delete innerEngines_.first;
		delete innerWorld_;
	}
	void CompleteConfiguration() {
		innerEngines_.Append(new InnerCatcher(this));
	}
	void Open() {
		std::for_each(inTranslators_.begin(),inTranslators_.end(),std::mem_fun(&InTranslator::Open));
		innerEngines_.first->Open();
	}
	void Process() {
		std::for_each(inTranslators_.begin(),inTranslators_.end(),std::mem_fun(&InTranslator::Process));
		innerEngines_.first->Process();
	}
	void Close() {
		std::for_each(inTranslators_.begin(),inTranslators_.end(),std::mem_fun(&InTranslator::Close));
		innerEngines_.first->Close();
	}
	void Pulse(const StrAttrs &attrs) {
		//std::for_each(inTranslators_.begin(),inTranslators_.end(),boost::bind(&OutTranslator::Pulse,_1,attrs));
		innerEngines_.first->Pulse(attrs);
	}
	void innerOpenOut() {
		std::for_each(outTranslators_.begin(),outTranslators_.end(),std::mem_fun(&OutTranslator::Open));
		this->NextOpen();
	}
	void innerProcessOut() {
		std::for_each(outTranslators_.begin(),outTranslators_.end(),std::mem_fun(&OutTranslator::Process));
		this->NextProcess();
	}
	void innerCloseOut() {
		std::for_each(outTranslators_.begin(),outTranslators_.end(),std::mem_fun(&OutTranslator::Close));
		this->NextClose();
	}
	void innerPulseOut(const StrAttrs &attrs) {
		//std::for_each(outTranslators_.begin(),outTranslators_.end(),boost::bind(&OutTranslator::Pulse,_1,attrs));
		this->NextPulse(attrs);
	}
};

} // namespace Dynagraph

#endif // InternalWorld_h
