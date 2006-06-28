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


#ifndef SEdger_h
#define SEdger_h

#include "common/LayoutToLayoutTranslator.h"

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout1,typename Layout2>
struct SEdger : ChangeTranslator<Layout1,Layout2> {
	LayoutToLayoutTranslator<Layout1,Layout2> actions_;
	SEdger(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2) 
		: ChangeTranslator<Layout1,Layout2>(world1,world2) {}
	void Process() {
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // SEdger_h
