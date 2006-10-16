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

#ifndef ColorByAgeConfigurator_h
#define ColorByAgeConfigurator_h

#include "Configurator.h"
#include "ColorByAge.h"

namespace Dynagraph {

struct ColorByAgeConfigurator {
	template<typename Configurators,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
		if(attrs.look("colorbyage")||attrs.look("agecolors"))
			engines.Append(new ColorByAge<Layout>(world));
		configureLayout<Configurators>(name,attrs,world,engines);
	}
};

} // namespace Dynagraph

#endif //ColorByAgeConfigurator_h
