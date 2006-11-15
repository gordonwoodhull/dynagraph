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
	template<typename Configurators,typename Source,typename Dest> 
	static bool Create(DString name,const StrAttrs &attrs,typename Data<Source>::type &source,typename Data<Dest>::type dest) {
		if(attrs.look("colorbyage")||attrs.look("agecolors"))
			engines.Append(new ColorByAge<Layout>(world));
		return createConfiguration<Configurators>(name,attrs,source,dest);
	}
};

} // namespace Dynagraph

#endif //ColorByAgeConfigurator_h
