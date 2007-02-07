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

#ifndef ChangeTranslator_h
#define ChangeTranslator_h

#include "ChangeTranslator.h"

namespace Dynagraph {

/* translators are different from processors in that
	* they have both a source and destination world
	* they are not chained
  the first difference is obvious.  the second one is debatable.
  it does not allow e.g. intermediate translations.
  it does make InternalWorld slightly easier to implement, but not so much so.
  we'll see.
*/
struct ChangeTranslating {
	virtual ~ChangeTranslating() = 0; // look! a virtual destructor that actually gets used!
	virtual void Open(ChangeTranslating *next) = 0;
	virtual void Process(ChangeTranslating *next) = 0;
	virtual void Close(ChangeTranslating *next) = 0;
	virtual void Pulse(ChangeTranslating *next,const StrAttrs &attrs) = 0;
};
template<typename SourceGraph,typename DestGraph>
struct ChangeTranslator : ChangeTranslating {
	ChangingGraph<SourceGraph> *sourceWorld_;
	ChangingGraph<DestGraph> *destWorld_;
	ChangeTranslator(ChangingGraph<SourceGraph> *sourceWorld,ChangingGraph<DestGraph> *destWorld)
		: sourceWorld_(sourceWorld),destWorld_(destWorld) {}
    virtual ~ChangeTranslator() {}

	virtual void Open(ChangeTranslating *next) {
		next->Open(0);
	}
	virtual void Process(ChangeTranslating *next) {
		next->Process(0);
	}
	virtual void Close(ChangeTranslating *next) {
		next->Close(0);
	}
	virtual void Pulse(ChangeTranslating *next,const StrAttrs &attrs) {
		next->Pulse(0,attrs);
	}
};

} // namespace Dynagraph

#endif // ChangeTranslator_h
