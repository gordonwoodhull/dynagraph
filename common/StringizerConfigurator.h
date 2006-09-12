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

#ifndef StringizerConfigurator_h
#define StringizerConfigurator_h

#include "Configurator.h"
#include "InternalTranslator.h"
#include "StringLayoutTranslator.h"

namespace Dynagraph {

// these globals should get folded into graph attributes, eliminating much useless code
extern Transform *g_transform;
extern bool g_useDotDefaults;


struct StringizerConfigurator {
	template<typename ConfiguratorVec,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
		// engine to translate strings to binary attrs
		typedef InternalTranslator2<Layout,StringToLayoutTranslator<Layout,Layout> > StringsInEngine;
		StringsInEngine *xlateIn = new StringsInEngine(engines.first->world_,StringToLayoutTranslator<Layout,Layout>(g_transform,g_useDotDefaults));
		engines.Prepend(xlateIn);

		// engine to translate binary attrs to strings
		typedef InternalTranslator2<Layout,LayoutToStringTranslator<Layout,Layout> > StringsOutEngine;
		StringsOutEngine *xlateOut = new StringsOutEngine(engines.first->world_,g_transform);
		engines.Append(xlateOut);
		configureLayout<ConfiguratorVec>(name,attrs,world,engines);
	}
};

} // namespace Dynagraph

#endif //StringizerConfigurator_h
