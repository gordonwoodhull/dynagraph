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

#ifndef RankerConfigurator_h
#define RankerConfigurator_h

#include "common/Configurator.h"
#include "NSRanker.h"

namespace Dynagraph {
namespace DynaDAG {

struct RankerConfigurator {
	template<typename Configurators,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
		if(attrs.look("layout","dynadag")=="dynadag" && attrs.look("ranker","nsranker")=="nsranker") // i dream of smarter rankers
			engines.Prepend(new NSRanker<Layout>(world));
		configureLayout<Configurators>(name,attrs,world,engines);
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif //RankerConfigurator_h
