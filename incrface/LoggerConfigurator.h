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

#ifndef LoggerConfigurator_h
#define LoggerConfigurator_h

#include "common/Configurator.h"
#include "common/InternalTranslator.h"
#include "common/StringLayoutTranslator.h"
#include "common/OutputIncrface.h"

// these globals should get folded into graph attributes, eliminating much useless code
extern Transform *g_transform;

struct LoggerConfigurator {
	template<typename Configurators,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
		typedef InternalTranslator2<InnerLayout,LayoutToStringTranslator<InnerLayout,InnerLayout> > MakeStrings;
		if(reports.enabled(dgr::inner_input)) {
			OutputIncrface<InnerLayout> *logIn = new OutputIncrface<InnerLayout>(innerWorld_,dgr::inner_input);
			engines.Prepend(logIn);
			MakeStrings *makeStrings = new MakeStrings(innerWorld_,g_transform);
			engines.Prepend(makeStrings);
		}
		if(reports.enabled(dgr::inner_output)) {
			MakeStrings *makeStrings = new MakeStrings(innerWorld_,g_transform);
			engines.Append(makeStrings);
			OutputIncrface<InnerLayout> *logOut = new OutputIncrface<InnerLayout>(innerWorld_,dgr::inner_output);
			engines.Append(logOut);
		}
		configureLayout<Configurators>(name,attrs,world,engines);
	}
};


} // namespace Dynagraph

#endif //LoggerConfigurator_h
