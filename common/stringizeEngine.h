/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/


#ifndef stringizeEngine_h
#define stringizeEngine_h

#include "common/InternalTranslator.h"
#include "common/StringLayoutTranslator.h"

namespace Dynagraph {

template<typename Layout>
EnginePair<Layout> stringizeEngine(EnginePair<Layout> engines,Transform *transform,bool useDotDefaults) {
	EnginePair<Layout> ret;
	typedef InternalTranslator2<Layout,StringToLayoutTranslator<Layout,Layout> > StringsInEngine;
	typedef InternalTranslator2<Layout,LayoutToStringTranslator<Layout,Layout> > StringsOutEngine;
	StringsInEngine *xlateIn = new StringsInEngine(StringToLayoutTranslator<Layout,Layout>(transform,useDotDefaults));
	StringsOutEngine *xlateOut = new StringsOutEngine(transform);
	xlateIn->next_ = engines.first;
	engines.second->next_ = xlateOut;
	return EnginePair<Layout>(xlateIn,xlateOut);
}

} // namespace Dynagraph
#endif // stringizeEngine_h
