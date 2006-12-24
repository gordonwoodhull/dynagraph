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

#ifndef FindChangeRectsConfigurator_h
#define FindChangeRectsConfigurator_h

#include "Configurator.h"
#include "FindChangeRect.h"

namespace Dynagraph {

struct FindChangeRectsConfigurator {
	template<typename Configurators,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
		if(attrs.look("findchangerects","false")=="true") {
			FCRData<Layout> *fcrdata = new FCRData<Layout>(world);
			FCRBefore<Layout> *fcrbefore = new FCRBefore<Layout>(fcrdata);
			FCRAfter<Layout> *fcrafter = new FCRAfter<Layout>(fcrdata);
			engines.Prepend(fcrbefore);
			engines.Append(fcrafter);
		}
		configureLayout<Configurators>(name,attrs,world,engines);
	}
};

} // namespace Dynagraph

#endif //FindChangeRectsConfigurator_h
