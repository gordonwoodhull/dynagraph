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

#ifndef createConfiguration_h
#define createConfiguration_h

#include "common/ChangingGraph.h"

namespace Dynagraph {

void createConfiguration(Name name,StrAttrs &attrs);
template<typename SourceLayout>
bool changeConfiguration(ChangingGraph<SourceLayout> *source,StrAttrs &attrs) {
	if(attrs.look("layout")) {
		createConfiguration(gd<Name>(&source->whole_),attrs);
		//for(typename SourceLayout::node_iter ni = source->whole_.nodes().begin(); ni!=source->whole_.nodes().end(); ++ni)
		return true;
	}
	return false;
}

} // namespace Dynagraph

#endif // createConfiguration_h
