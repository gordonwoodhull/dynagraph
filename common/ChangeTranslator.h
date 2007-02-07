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

// translators are different from processors in that they have both a source and destination world
template<typename SourceGraph,typename DestGraph>
struct ChangeTranslator : ChangeProcessor {
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
